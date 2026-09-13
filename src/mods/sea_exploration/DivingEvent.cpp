#include "DivingEvent.h"
#include "utils/GameHelper.h"
#include "memory/MemoryHelper.h"
#include <cmath>
#include <cstdlib>

const static bool DEBUG_DIVING_EVENT = false;

static long long DistanceSquared(const LongVector3& p1, const LongVector3& p2)
{
	auto diffx = (p2.x - p1.x);
	auto diffy = (p2.y - p1.y);
	auto diffz = (p2.z - p1.z);
	return diffx * diffx + diffy * diffy + diffz * diffz;
}

cube::DivingEvent::DivingEvent(bool* autoGoldUsage)
	: Event(), m_ItemEffectTimer(nullptr)
{
	this->autoGoldUsage = autoGoldUsage;
	eventType = cube::EventType::Diving;

	m_SpawnTimer = cube::Timer(FISH_SPAWN_INTERVAL, m_CurrentTime);
	m_TreasureTimer = cube::Timer(TREASURE_SPAWN_INTERVAL, m_CurrentTime);
	m_BossTimer = cube::Timer(BOSS_SPAWN_INTERVAL, m_CurrentTime);
	m_BoundsTimer = cube::Timer(BOUNDS_CHECK_INTERVAL, m_CurrentTime);

	SetDiving(true);
}

cube::DivingEvent::~DivingEvent()
{
	cube::Game* game = cube::GetGame();
	if (game == nullptr || game->host.running == false)
	{
		return;
	}

	auto map = &game->host.world.id_to_creature_map;
	if (m_ItemEffectTimer != nullptr)
	{
		delete m_ItemEffectTimer;
	}

	for (auto& p : m_SpawnedCreatures)
	{
		for (auto pair : p.creature_ids)
		{
			auto creature = map->find(pair.id);
			if (creature != map->end())
			{
				creature->second->entity_data.HP = 0;
				map->erase(pair.id);
			}
		}
	}

	for (auto pair : m_SpawnedTreasures)
	{
		auto creature = map->find(pair.id);
		if (creature != map->end())
		{
			creature->second->entity_data.HP = 0;
			map->erase(pair.id);
		}
	}

	for (auto pair : m_SpawnedBosses)
	{
		auto creature = map->find(pair.id);
		if (creature != map->end())
		{
			creature->second->entity_data.HP = 0;
			map->erase(pair.id);
		}
	}

	SetDiving(false);
}

void cube::DivingEvent::Update()
{
	Event::Update();
	HandleTreasureTimer();
	HandleFishTimer();
	HandleBossTimer();
	HandleBoundsCheckTimer();
	UpdateIds();
}

void cube::DivingEvent::UpdateIds()
{
	for (auto& p : m_SpawnedCreatures)
	{
		for (size_t i = 0; i < p.creature_ids.size(); i++)
		{
			p.creature_ids[i].id = cube::CreatureFactory::CheckAndUpdateID(p.creature_ids[i].id);
			if (p.creature_ids[i].id == -1)
			{
				if (p.creature_ids[i].creature)
				{
					p.creature_ids[i].creature->entity_data.HP = 0;
				}
				p.creature_ids.erase(p.creature_ids.begin() + i);
				i--;
			}
		}
	}
}

void cube::DivingEvent::SetDiving(bool diving)
{
	if (diving)
	{
		MemoryHelper::WriteByte((char*)CWBase() + 0x2E038D + 0x02, 0x58);
	}
	else
	{
		MemoryHelper::WriteByte((char*)CWBase() + 0x2E038D + 0x02, 0x5C);
	}
}

void cube::DivingEvent::HandleItemEffectTimer()
{
	if (m_ItemEffectTimer != nullptr && m_ItemEffectTimer->IsTriggered(m_CurrentTime))
	{
		delete m_ItemEffectTimer;
		m_ItemEffectTimer = nullptr;
		SetDiving(false);
	}

	if (m_ItemEffectTimer == nullptr)
	{
		ConsumeItem();
	}
}

void cube::DivingEvent::ConsumeItem()
{
	if (autoGoldUsage && !*autoGoldUsage)
	{
		return;
	}

	cube::Game* game = cube::GetGame();
	if (!game) return;
	cube::Creature* player = game->GetPlayer();
	if (!player) return;

	if (player->gold >= 10)
	{
		player->gold -= 10;
		m_ItemEffectTimer = new cube::Timer(10.0, m_CurrentTime);
		game->PrintMessage(L"[Consumed] ", 100, 255, 0);
		game->PrintMessage(L"10 Gold\n");
	}
}

void cube::DivingEvent::HandleFishTimer()
{
	cube::Game* game = cube::GetGame();
	if (!game || !game->GetPlayer()) return;
	LongVector3 pos = game->GetPlayer()->entity_data.position;

	if (m_SpawnTimer.IsTriggered(m_CurrentTime) && m_SpawnedCreatures.size() < MAX_FISH_COUNT / FISH_SPAWN_AMOUNT)
	{
		SpawnFishes(pos);
	}
}

void cube::DivingEvent::SpawnFishes(const LongVector3& position)
{
	std::vector<cube::Creature*> creatures = cube::CreatureFactory::SpawnFishes(FISH_SPAWN_AMOUNT, CREATURE_SPAWN_RANGE);

	std::vector<IdCreaturePair> ids;
	for (auto creature : creatures)
	{
		if (creature)
		{
			ids.push_back({ creature->id, creature });
		}
	}
	m_SpawnedCreatures.push_back({ position, ids });
}

void cube::DivingEvent::HandleTreasureTimer()
{
	if (m_TreasureTimer.IsTriggered(m_CurrentTime) && m_SpawnedTreasures.size() < MAX_TREASURE_COUNT)
	{
		cube::Game* game = cube::GetGame();
		if (!game || !game->GetPlayer()) return;
		LongVector3 pos = game->GetPlayer()->entity_data.position;
		SpawnTreasures(pos);
	}
}

void cube::DivingEvent::SpawnTreasures(const LongVector3& position)
{
	long long distance = static_cast<long long>((2.f + 1.f * Helper::RandomZeroToOne()) * CREATURE_SPAWN_RANGE);
	LongVector3 offset = cube::CreatureFactory::GetRandomOffset(distance);
	offset.x += position.x;
	offset.y += position.y;
	offset.z = position.z - std::abs(offset.z);

	if (!Helper::PositionContainsWater(offset))
	{
		return;
	}
	
	cube::Game* game = cube::GetGame();
	if (!game || !game->GetPlayer()) return;

	int type = GenerateChestType();
	cube::Creature* creature = cube::CreatureFactory::SpawnChest(
		offset, 
		game->GetPlayer()->entity_data.current_region, 
		type
	);

	if (creature != nullptr)
	{
		m_SpawnedTreasures.push_back({ creature->id, creature });
		game->PrintMessage(L"A treasure chest appeared!\n", 255, 165, 0);
	}
}

int cube::DivingEvent::GenerateChestType()
{
	const static int NORMAL_CHANCE = 70;
	const static int SKULL_CHANCE = 20;
	const static int OBSEDIAN_CHANCE = 5;

	int random = rand() % 100;

	if (random < NORMAL_CHANCE)
	{
		return 0;
	}
	else if (random - NORMAL_CHANCE < SKULL_CHANCE)
	{
		return 1;
	}
	else if (random - NORMAL_CHANCE - SKULL_CHANCE < OBSEDIAN_CHANCE)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

void cube::DivingEvent::HandleBossTimer()
{
	if (m_BossTimer.IsTriggered(m_CurrentTime) && m_SpawnedBosses.size() < MAX_BOSS_COUNT)
	{
		cube::Game* game = cube::GetGame();
		if (!game || !game->GetPlayer()) return;
		LongVector3 pos = game->GetPlayer()->entity_data.position;
		SpawnBoss(pos);
	}
}

void cube::DivingEvent::SpawnBoss(const LongVector3& position)
{
	long long distance = static_cast<long long>((2.f + 1.f * Helper::RandomZeroToOne()) * CREATURE_SPAWN_RANGE);
	LongVector3 offset = cube::CreatureFactory::GetRandomOffset(distance);
	offset.x += position.x;
	offset.y += position.y;
	offset.z = position.z - std::abs(offset.z);

	if (Helper::PositionContainsWater(offset))
	{
		cube::Game* game = cube::GetGame();
		if (!game || !game->GetPlayer()) return;

		cube::Creature* boss = cube::CreatureFactory::SpawnBoss(offset, game->GetPlayer()->entity_data.current_region);
		if (boss != nullptr)
		{
			m_SpawnedBosses.push_back({ boss->id, boss });
			game->PrintMessage(L"An underwater boss appeared!\n", 255, 165, 0);
		}
	}
}

void cube::DivingEvent::HandleBoundsCheckTimer()
{
	if (m_BoundsTimer.IsTriggered(m_CurrentTime))
	{
		cube::Game* game = cube::GetGame();
		if (!game || !game->GetPlayer()) return;
		LongVector3 pos = game->GetPlayer()->entity_data.position;
		BoundCheckFishes(pos);
		BoundCheckTreasures(pos);
		BoundCheckBosses(pos);
	}
}

void cube::DivingEvent::BoundCheckFishes(const LongVector3& position)
{
	const static auto dist = (BOUNDS_CHECK_DIST_MULTIPLIER * CREATURE_SPAWN_RANGE) * (BOUNDS_CHECK_DIST_MULTIPLIER * CREATURE_SPAWN_RANGE);
	cube::Game* game = cube::GetGame();
	if (!game) return;
	auto map = &game->host.world.id_to_creature_map;
	std::vector<int> toBeErased;
	for (size_t i = 0; i < m_SpawnedCreatures.size(); i++)
	{
		auto p = m_SpawnedCreatures.at(i);
		if (DistanceSquared(p.position, position) < dist)
		{
			continue;
		}

		for (auto pair : p.creature_ids)
		{
			auto creature = map->find(pair.id);
			if (creature != map->end())
			{
				creature->second->entity_data.HP = 0;
				map->erase(pair.id);
			}
		}

		toBeErased.push_back(static_cast<int>(i));
	}

	for (int i = static_cast<int>(toBeErased.size()) - 1; i >= 0; i--)
	{
		m_SpawnedCreatures.erase(m_SpawnedCreatures.begin() + toBeErased.at(i));
	}
}

void cube::DivingEvent::BoundCheckTreasures(const LongVector3& position)
{
	const static auto distTreasure = (BOUNDS_CHECK_DIST_MULTIPLIER * 3 * CREATURE_SPAWN_RANGE) * (BOUNDS_CHECK_DIST_MULTIPLIER * 3 * CREATURE_SPAWN_RANGE);
	cube::Game* game = cube::GetGame();
	if (!game) return;
	auto map = &game->host.world.id_to_creature_map;

	std::vector<int> toBeErased;
	for (size_t i = 0; i < m_SpawnedTreasures.size(); i++)
	{
		auto pair = m_SpawnedTreasures.at(i);
		auto creature = map->find(pair.id);
		if (creature != map->end())
		{
			if (DistanceSquared(creature->second->entity_data.position, position) < distTreasure)
			{
				continue;
			}
			creature->second->entity_data.HP = 0;
			map->erase(pair.id);
			toBeErased.push_back(static_cast<int>(i));
		}
		else
		{
			toBeErased.push_back(static_cast<int>(i));
		}
	}

	for (int i = static_cast<int>(toBeErased.size()) - 1; i >= 0; i--)
	{
		m_SpawnedTreasures.erase(m_SpawnedTreasures.begin() + toBeErased.at(i));
	}
}

void cube::DivingEvent::BoundCheckBosses(const LongVector3& position)
{
	const static auto distBoss = (BOUNDS_CHECK_DIST_MULTIPLIER * 3 * CREATURE_SPAWN_RANGE) * (BOUNDS_CHECK_DIST_MULTIPLIER * 3 * CREATURE_SPAWN_RANGE);
	cube::Game* game = cube::GetGame();
	if (!game) return;
	auto map = &game->host.world.id_to_creature_map;

	std::vector<int> toBeErased;
	for (size_t i = 0; i < m_SpawnedBosses.size(); i++)
	{
		auto pair = m_SpawnedBosses.at(i);
		auto creature = map->find(pair.id);
		if (creature != map->end())
		{
			auto p = creature->second;
			if (p->entity_data.HP <= 0.f)
			{
				toBeErased.push_back(static_cast<int>(i));
				map->erase(p->id);
				continue;
			}

			if (DistanceSquared(p->entity_data.position, position) >= distBoss)
			{
				map->erase(p->id);
				p->entity_data.HP = 0;
				toBeErased.push_back(static_cast<int>(i));
			}
		}
		else
		{
			toBeErased.push_back(static_cast<int>(i));
		}
	}

	for (int i = static_cast<int>(toBeErased.size()) - 1; i >= 0; i--)
	{
		m_SpawnedBosses.erase(m_SpawnedBosses.begin() + toBeErased.at(i));
	}
}
