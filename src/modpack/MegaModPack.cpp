#include "MegaModPack.h"
#include "CommandHandler.h"
#include "utils/GameHelper.h"
#include "memory/MemoryHelper.h"

// 12 Standalone Sub-Mods
#include "sea_exploration/SeaExplorationMod.h"
#include "lore_interactions/LoreInteractionMod.h"
#include "combat_updates/CombatUpdateMod.h"
#include "creature_updates/CreatureUpdatesMod.h"
#include "shop_updates/ShopUpdateMod.h"
#include "world_gen/WorldGenMod.h"
#include "beginner_mode/BeginnerModeMod.h"
#include "region_lock/RegionLockMod.h"
#include "weapon_upgrades/WeaponUpgradeMod.h"
#include "quest_system/QuestMod.h"
#include "player_updates/PlayerUpdatesMod.h"
#include "stack_updates/StackUpdatesMod.h"

#include <cwchar>
#include <vector>

static MegaModPack* s_MegaModPackInstance = nullptr;

extern "C" {
	void* ASMOnCreatureDeath_jmpback = nullptr;
	void* ASMOnTalkToCreature_jmpback = nullptr;
	void* ASMOnTalkToCreature_jmpback2 = nullptr;
	void* ASMOnTalkToCreature_bail = nullptr;

	void ASMOnCreatureDeath();
	void ASMOnTalkToCreature();

	void OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker)
	{
		if (!attacker)
		{
			cube::Game* game = cube::GetGame();
			if (game)
			{
				attacker = game->GetPlayer();
			}
		}

		if (s_MegaModPackInstance)
		{
			for (BaseMod* mod : s_MegaModPackInstance->GetMods())
			{
				if (mod && mod->m_Enabled)
				{
					mod->OnCreatureDeath(creature, attacker);
				}
			}
		}
	}

	int OnTalkToCreature(cube::Game* game, cube::Creature* creature)
	{
		if (!s_MegaModPackInstance || !game || !creature) return 0;

		for (BaseMod* mod : s_MegaModPackInstance->GetMods())
		{
			if (mod && mod->m_Enabled)
			{
				if (mod->OnCreatureTalk(game, creature))
				{
					return 1;
				}
			}
		}
		return 0;
	}
}

MegaModPack::MegaModPack()
{
	s_MegaModPackInstance = this;

	m_Mods.push_back(new SeaExplorationMod());
	m_Mods.push_back(new LoreInteractionMod());
	m_Mods.push_back(new CombatUpdateMod());
	m_Mods.push_back(new CreatureUpdatesMod());
	m_Mods.push_back(new ShopUpdateMod());
	m_Mods.push_back(new WorldGenMod());
	m_Mods.push_back(new BeginnerModeMod());
	m_Mods.push_back(new RegionLockMod());
	m_Mods.push_back(new WeaponUpgradeMod());
	m_Mods.push_back(new QuestMod());
	m_Mods.push_back(new PlayerUpdatesMod());
	m_Mods.push_back(new StackUpdatesMod());
}

MegaModPack::~MegaModPack()
{
	for (BaseMod* mod : m_Mods)
	{
		delete mod;
	}
	m_Mods.clear();

	if (s_MegaModPackInstance == this)
	{
		s_MegaModPackInstance = nullptr;
	}
}

void MegaModPack::Initialize()
{
	ASMOnCreatureDeath_jmpback = (void*)CWOffset(0x29E4A5);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x29E494), (void*)&ASMOnCreatureDeath);

	ASMOnTalkToCreature_jmpback = (void*)CWOffset(0x98D15);
	ASMOnTalkToCreature_jmpback2 = (void*)CWOffset(0x98ED2);
	ASMOnTalkToCreature_bail = (void*)CWOffset(0x988C1);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x98D04), (void*)&ASMOnTalkToCreature);

	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->Initialize();
		}
	}
}

void MegaModPack::OnGameTick(cube::Game* game)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnGameTick(game);
		}
	}
}

int MegaModPack::OnChat(std::wstring* message)
{
	if (!message) return 0;

	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			if (mod->OnChat(message))
			{
				return 1;
			}
		}
	}
	return 0;
}

void MegaModPack::OnGetKeyboardState(BYTE* diKeys)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnGetKeyboardState(diKeys);
		}
	}
}

void MegaModPack::OnCreatureArmorCalculated(cube::Creature* creature, float* armor)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureArmorCalculated(creature, armor);
		}
	}
}

void MegaModPack::OnCreatureCriticalCalculated(cube::Creature* creature, float* critical)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureCriticalCalculated(creature, critical);
		}
	}
}

void MegaModPack::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureAttackPowerCalculated(creature, power);
		}
	}
}

void MegaModPack::OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureSpellPowerCalculated(creature, power);
		}
	}
}

void MegaModPack::OnCreatureHasteCalculated(cube::Creature* creature, float* haste)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureHasteCalculated(creature, haste);
		}
	}
}

void MegaModPack::OnCreatureHPCalculated(cube::Creature* creature, float* hp)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureHPCalculated(creature, hp);
		}
	}
}

void MegaModPack::OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureResistanceCalculated(creature, resistance);
		}
	}
}

void MegaModPack::OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureRegenerationCalculated(creature, regeneration);
		}
	}
}

void MegaModPack::OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration)
{
	for (BaseMod* mod : m_Mods)
	{
		if (mod && mod->m_Enabled)
		{
			mod->OnCreatureManaGenerationCalculated(creature, manaGeneration);
		}
	}
}

EXPORT GenericMod* MakeMod() {
	return new MegaModPack();
}
