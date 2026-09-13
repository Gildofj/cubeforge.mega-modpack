#include "SeaExplorationMod.h"
#include "utils/GameHelper.h"
#include "memory/MemoryHelper.h"
#include <cwchar>

// Hooks ASM
extern "C" {
	void* ASMIsChestOpen_jmpback = nullptr;
	void* ASMIsChestOpen_bail = nullptr;
	void* ASMChestInteractionHandler_jmpback = nullptr;

	void ASMIsChestOpen();
	void ASMChestInteractionHandler();
}

static SeaExplorationMod* s_Instance = nullptr;

extern "C" void OnChestInteraction(cube::Game* game, cube::Creature* creature)
{
	if (!game || !creature || !s_Instance) return;

	int type = creature->entity_data.race - 181;
	if (!s_Instance->OnChestInteraction(game, creature, type))
	{
		return;
	}

	unsigned long long mask = (1ULL << static_cast<int>(cube::Enums::StateFlags::VisibleOnMap));
	if ((creature->entity_data.binary_toggles & mask) != 0)
	{
		creature->entity_data.binary_toggles &= ~mask;
	}

	creature->entity_data.interaction_state = 2;
}

static void SetupChestHooks()
{
	ASMIsChestOpen_jmpback = (void*)CWOffset(0x988C1);
	ASMIsChestOpen_bail = (void*)CWOffset(0x99288);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x988A7), (void*)&ASMIsChestOpen);

	ASMChestInteractionHandler_jmpback = (void*)CWOffset(0x9928D);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x99288), (void*)&ASMChestInteractionHandler);
}

SeaExplorationMod::SeaExplorationMod()
	: BaseMod("Sea Exploration Mod", "cubeforge-sea-exploration", 1, { 1, 3, 0 }),
	  m_DivingEvent(nullptr)
{
	m_Data = { true };
	Load(&m_Data, sizeof(m_Data));
	s_Instance = this;
}

SeaExplorationMod::~SeaExplorationMod()
{
	if (m_DivingEvent)
	{
		delete m_DivingEvent;
		m_DivingEvent = nullptr;
	}
	if (s_Instance == this)
	{
		s_Instance = nullptr;
	}
}

void SeaExplorationMod::Initialize()
{
	cube::Helper::DisableCreatureFloating();
	SetupChestHooks();
}

int SeaExplorationMod::OnChestInteraction(cube::Game* game, cube::Creature* creature, int type)
{
	if (!game || !creature) return 0;

	game->PrintMessage(L"You opened a treasure chest!\n", 255, 165, 0);
	cube::Creature* player = game->GetPlayer();
	if (!player) return 0;

	switch (type)
	{
	case 1: // Skull chest
	{
		if (cube::Helper::RandomInt() % 20 == 0)
		{
			cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Artifact);
		}
		cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Gold, 5 * (1 + cube::Helper::RandomInt() % 6));
		int cnt = 1 + cube::Helper::RandomInt() % 2;
		for (int i = 0; i < cnt; i++)
		{
			cube::Item item = cube::Helper::GenerateItem(cube::Helper::ItemGenerationType::Gear, creature->entity_data.current_region);
			item.rarity = 3 + cube::Helper::RandomInt() % 5;
			cube::Helper::DropItem(player, item, 1);
		}
		break;
	}
	case 2: // Obsidian chest
		cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Artifact);
		break;
	case 3: // Bone chest
		cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Pet);
		break;
	case 0:
	default: // Normal chest
		cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Gold, 10);
		if (cube::Helper::RandomInt() % 4 != 0)
		{
			cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Gear, 1);
		}
		if (cube::Helper::RandomInt() % 2 == 0)
		{
			cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Consumable, 1);
		}
		break;
	}

	return 1;
}

void SeaExplorationMod::OnGameTick(cube::Game* game)
{
	if (!game || cube::Helper::InGUI(game) || !game->host.running) return;

	cube::Creature* player = game->GetPlayer();
	if (!player) return;

	unsigned int flags = player->entity_data.flags;
	if ((flags & (1 << static_cast<int>(cube::Enums::CollisionFlags::Water))) &&
		!(flags & (1 << static_cast<int>(cube::Enums::CollisionFlags::Surfaced))))
	{
		if (m_DivingEvent == nullptr)
		{
			m_DivingEvent = new cube::DivingEvent(&m_Data.autoGoldUsage);
		}
	}
	else if (m_DivingEvent != nullptr)
	{
		delete m_DivingEvent;
		m_DivingEvent = nullptr;
	}

	if (m_DivingEvent != nullptr)
	{
		m_DivingEvent->Update();
	}
}

int SeaExplorationMod::OnChat(std::wstring* message)
{
	if (!message) return 0;
	const wchar_t* msg = message->c_str();

	int val;
	if (swscanf_s(msg, L"/cubeforge sea autogold %d", &val) == 1)
	{
		m_Data.autoGoldUsage = (val != 0);
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(m_Data.autoGoldUsage ? L"[SeaExploration] Auto-gold ativado.\n" : L"[SeaExploration] Auto-gold desativado.\n", 100, 255, 100);
		return 1;
	}

	if (!wcscmp(msg, L"/enable autogoldusage"))
	{
		m_Data.autoGoldUsage = true;
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(L"[SeaExploration] Auto-gold ativado.\n", 100, 255, 100);
		return 1;
	}

	if (!wcscmp(msg, L"/disable autogoldusage"))
	{
		m_Data.autoGoldUsage = false;
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(L"[SeaExploration] Auto-gold desativado.\n", 100, 255, 100);
		return 1;
	}

	return 0;
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new SeaExplorationMod();
}
#endif
