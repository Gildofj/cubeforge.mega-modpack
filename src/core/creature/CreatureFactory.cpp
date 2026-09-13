#include "CreatureFactory.h"
#include <random>
#include <ctime>
#include <climits>
#include <cmath>

#include "Creature.h"
#include "../utils/GameHelper.h"

static std::vector<__int64> s_InvicibleIds;

void cube::CreatureFactory::AddInvisID(__int64 id)
{
	for (auto i : s_InvicibleIds)
	{
		if (i == id)
		{
			return;
		}
	}
	s_InvicibleIds.push_back(id);
}

void cube::CreatureFactory::PrintInvisIDs()
{
}

__int64 cube::CreatureFactory::CheckAndUpdateID(__int64 id)
{
	cube::Game* game = cube::GetGame();
	if (!game || !game->host.running || !game->world)
	{
		return id;
	}

	auto map = &game->host.world.id_to_creature_map;
	auto list = &game->host.world.creatures;
	auto local_list = &game->world->creatures;

	if (map->find(id) == map->end())
	{
		for (auto creature : *list)
		{
			if (creature && creature->id == id)
			{
				AddInvisID(id);
				__int64 newId = GenerateId();
				creature->id = newId;
				map->insert_or_assign(newId, creature);
				return newId;
			}
		}
		
		for (auto creature : *local_list)
		{
			if (creature && creature->id == id)
			{
				creature->entity_data.HP = 0;
			}
		}
	}

	return id;
}

__int64 cube::CreatureFactory::GenerateId()
{
	cube::Game* game = cube::GetGame();
	if (!game) return -2;
	auto list = &game->host.world.creatures;
	__int64 min = LLONG_MAX;
	for (auto creature : *list)
	{
		if (!creature) continue;
		long long id = creature->id;
		if (id >= 0 && id < 1000)
		{
			continue;
		}
		min = id < min ? id : min;
	}
	min--;
	if (std::abs(min) <= 1)
	{
		return -2;
	}
	return min;
}

void cube::CreatureFactory::SetAppearance(cube::Creature* creature, int entityType, int entityBehaviour, int level)
{
	if (!creature) return;
	creature->entity_data.race = entityType;
	creature->entity_data.hostility_type = entityBehaviour;
	creature->entity_data.level = level;
	cube::Game* game = cube::GetGame();
	if (game)
	{
		((void (*)(cube::World*, int, cube::Creature*))CWOffset(0x2B67B0))(&game->host.world, 0, creature);
	}
}

void cube::CreatureFactory::AddCreatureToWorld(cube::Creature* creature)
{
	if (!creature) return;
	cube::Game* game = cube::GetGame();
	if (!game) return;

	game->host.world.creatures.push_back(creature);
	game->host.world.id_to_creature_map.insert_or_assign(creature->id, creature);
}

cube::Creature* cube::CreatureFactory::SpawnCreature(const LongVector3& position, const IntVector2& region, int entityType, int entityBehaviour, int level)
{
	cube::Creature* creature = cube::Creature::Create(cube::CreatureFactory::GenerateId());
	if (creature != nullptr)
	{
		creature->entity_data.position = position;
		creature->entity_data.current_region = region;
		creature->entity_data.yaw = Helper::RandomZeroToOne() * 360;
		cube::CreatureFactory::SetAppearance(creature, entityType, entityBehaviour, level);
		
		auto inv = &creature->inventory_tabs;
		for (auto& vector : *inv)
		{
			for (auto& itemstack : vector)
			{
				itemstack.item.region = creature->entity_data.current_region;
			}
		}
		cube::CreatureFactory::AddCreatureToWorld(creature);
	}
	return creature;
}

cube::Creature* cube::CreatureFactory::SpawnChest(const LongVector3& position, const IntVector2& region, int chestType, int level)
{
	if (chestType < 0 || chestType >= 4)
	{
		return nullptr;
	}

	if (level < 0)
	{
		level = static_cast<int>(Helper::RandomZeroToOne() * 6);
	}

	cube::Creature* creature = SpawnCreature(position, region, 181 + chestType, (int)cube::Enums::EntityBehaviour::ExamineObject, level);
	if (creature != nullptr)
	{
		creature->entity_data.binary_toggles |= 1 << (int)cube::Enums::StateFlags::ActiveLantern;
		creature->entity_data.binary_toggles |= 1 << (int)cube::Enums::StateFlags::VisibleOnMap;
		creature->entity_data.interaction_state = 1;
	}

	return creature;
}

cube::Creature* cube::CreatureFactory::SpawnFish(const LongVector3& position, const IntVector2& region, int entityType, int level, int friendly)
{
	const static int ENTITY_COUNT = 13;
	const static int MAX_LEVEL = 5;
	const static int ENTITY_TYPES[ENTITY_COUNT] = {
		106, 107, 145, 146, 147, 148, 149, 150, 152, 153, 154, 155, 295
	};

	int random = std::rand();
	if (entityType == -1)
	{
		entityType = random % ENTITY_COUNT;
	}

	if (level == -1)
	{
		level = random % MAX_LEVEL;
	}

	if (friendly == -1)
	{
		friendly = random % 2;
	}

	cube::Creature* creature = SpawnCreature(
		position,
		region,
		ENTITY_TYPES[entityType],
		(int)cube::Enums::EntityBehaviour::Hostile,
		level
	);

	if (creature != nullptr)
	{
		creature->entity_data.appearance.flags2 |= friendly << (int)cube::Enums::AppearanceModifiers::IsFriendly;
		creature->entity_data.HP = creature->GetMaxHP();
	}

	return creature;
}

cube::Creature* cube::CreatureFactory::SpawnBoss(const LongVector3& position, const IntVector2& region)
{
	const static int IDS[] =
	{
		82,  // Minotaur
		108, // Troll
		109, // Dark troll
		110, // Hell demon
		111, // Golem
		112, // Ember golem
		115, // Cyclops
		118, // Runegiant
		119, // Saurian
		150, // Shark
		215, // Turtle (blue)
		216, // Snow gnobold
		229, // Crawler
		254, // Half-demon
		263, // Treant
		269, // Monstosity
		295, // Snail
	};

	const static float SCALES[] =
	{
		3.0f, // Minotaur
		2.0f, // Troll
		2.0f, // Dark troll
		2.0f, // Hell demon
		2.0f, // Golem
		2.0f, // Ember golem
		2.0f, // Cyclops
		2.0f, // Runegiant
		2.0f, // Saurian
		4.0f, // Shark
		5.0f, // Turtle (blue)
		5.0f, // Snow gnobold
		6.0f, // Crawler
		6.0f, // Half-demon
		2.0f, // Treant
		3.0f, // Monstosity
		6.0f, // Snail
	};

	int count = sizeof(IDS) / sizeof(*IDS);
	int bossType = cube::Helper::RandomInt() % count;

	int level = 3 + cube::Helper::RandomInt() % 3;
	cube::Creature* boss = SpawnCreature(position, region, IDS[bossType], (int)cube::Enums::EntityBehaviour::Hostile, level);
	if (boss == nullptr)
	{
		return boss;
	}

	int random = cube::Helper::RandomInt() % 100;
	if (random < 60)
	{
		boss->entity_data.appearance.flags2 |= 1 << (int)cube::Enums::AppearanceModifiers::IsMiniBoss;
	}

	if (random > 40)
	{
		boss->entity_data.appearance.flags2 |= 1 << (int)cube::Enums::AppearanceModifiers::IsBoss;
	}

	if (random % 4 == 0)
	{
		boss->entity_data.appearance.flags2 |= 1 << (int)cube::Enums::AppearanceModifiers::IsNamedBoss;
	}

	boss->entity_data.binary_toggles |= 1 << (int)cube::Enums::StateFlags::VisibleOnMap;
	boss->entity_data.HP = boss->GetMaxHP();

	boss->entity_data.appearance.graphics_scale *= SCALES[bossType];
	boss->entity_data.appearance.hitbox_scale *= SCALES[bossType];
	boss->entity_data.appearance.physics_scale *= SCALES[bossType];

	return boss;
}

std::vector<cube::Creature*> cube::CreatureFactory::SpawnFishes(int amount, long long range)
{
	std::vector<cube::Creature*> creatures;
	if (amount <= 0) return creatures;

	cube::Game* game = cube::GetGame();
	if (!game) return creatures;

	cube::Creature* player = game->GetPlayer();
	if (!player) return creatures;

	LongVector3 position = player->entity_data.position;
	for (int i = 0; i < amount; i++)
	{
		LongVector3 offset = cube::CreatureFactory::GetRandomOffset(range);
		offset.x += position.x;
		offset.y += position.y;
		offset.z += position.z;
		if (Helper::PositionContainsWater(offset))
		{
			cube::Creature* creature = cube::CreatureFactory::SpawnFish(offset, player->entity_data.current_region, -1, -1, 0);
			if (creature != nullptr)
			{
				creatures.push_back(creature);
			}
		}
	}

	return creatures;
}

LongVector3 cube::CreatureFactory::GetRandomOffset(long long range)
{
	auto t = Helper::RandomZeroToOne() * 2 * 3.14159265f;
	auto rr = range;
	auto cost = std::cos(t);
	auto sint = std::sin(t);
	return LongVector3(static_cast<long long>(rr * cost), static_cast<long long>(rr * sint), static_cast<long long>(rr * cost * sint));
}
