#include "main.h"
#include <stdlib.h>

#include "src/cwsdk-extension.h"

#include "src/mods/SeaExplorationMod/SeaExplorationMod.h"
#include "src/mods/LoreInteractionMod/LoreInteractionMod.h"
#include "src/mods/CombatUpdatesMod/CombatUpdateMod.h"
#include "src/mods/CreatureUpdatesMod/CreatureUpdatesMod.h"
#include "src/mods/ShopUpdateMod/ShopUpdateMod.h"
#include "src/mods/WorldGenMod/WorldGenMod.h"
#include "src/mods/BeginnerModeMod/BeginnerModeMod.h"
#include "src/mods/RegionLockUpdateMod/RegionLockUpdateMod.h"
#include "src/mods/WeaponUpgradeMod/WeaponUpgradeMod.h"
#include "src/mods/QuestMod/QuestMod.h"
#include "src/mods/PlayerUpdatesMod/PlayerUpdatesMod.h"
#include "src/mods/StackUpdatesMod/StackUpdatesMod.h"
#include "src/CubeMod.h"


#include "src/hooks/ChestInteractionHandler.h"
#include "src/hooks/ShopInteractionHandler.h"
#include "src/hooks/ItemPriceHandler.h"
#include "src/hooks/lore_increase.h"
#include "src/hooks/ItemDropPatch.h"
#include "src/hooks/CreatureDeathHandler.h"
#include "src/hooks/CreatureTalkHandler.h"

// OLD
#define DEBUG 1

// OLD
int DisplayOnlyInDebugMessage()
{
	cube::GetGame()->PrintMessage(L"[Error] This command is only available in debug mode.\n", 255, 127, 80);
	return 1;
}

/* Mod class containing all the functions for the mod.
*/
class Mod : public GenericMod {
private:
	std::vector<CubeMod*> modVector;
	std::vector<hook::HookEventData> hookEvents;

public:
	Mod() = default;

	virtual ~Mod() {
		for (CubeMod* m : modVector) {
			delete m;
		}
		modVector.clear();
		g_Mods.clear();
	}

	/* Hook for the chat function. Triggers when a user sends something in the chat.
	 * @param	{std::wstring*} message
	 * @return	{int}
	*/
	virtual int OnChat(std::wstring* message) override {
		if (!message) return 0;

		const wchar_t* msg = message->c_str();
		int ID, value;
		if (swscanf_s(msg, L"/mod %d %d", &ID, &value) == 2)
		{
			for (CubeMod* mod : modVector)
			{
				if (mod && mod->m_ID == ID)
				{
					mod->m_Enabled = (value != 0);
					std::string tmp = (mod->m_Enabled ? "Enabled: " : "Disabled: ");
					tmp += mod->m_Name;
					Popup("Notice", tmp.c_str());
				}
			}
			cube::SaveSettings(&modVector);
			return 1;
		}

		if (swscanf_s(msg, L"/class %d", &ID) == 1)
		{
			cube::Game* game = cube::GetGame();
			if (!game) return 0;
			cube::Creature* player = game->GetPlayer();
			if (!player) return 0;

			cube::Creature* creature = cube::CreatureFactory::SpawnCreature(player->entity_data.position, player->entity_data.current_region,
				304, (int)cube::Enums::EntityBehaviour::NPC, 1);
			if (creature)
			{
				creature->entity_data.appearance.flags2 |= 1 << (int)cube::Enums::AppearanceModifiers::NeededForGemTrader;
				creature->entity_data.classType = ID;
				creature->entity_data.specialization = 1;
			}
			return 0;
		}

		int type;
		if (swscanf_s(msg, L"/t %d", &type) == 1)
		{
			cube::Game* game = cube::GetGame();
			if (!game) return 0;
			cube::Creature* player = game->GetPlayer();
			if (!player) return 0;

			cube::Creature::AnimationState* animationState = &player->animation_state;
			animationState->current_animation_state_timer = 0.f;
			animationState->current_animation_state_id = type;
			return 1;
		}

		for (CubeMod* mod : g_Mods)
		{
			if (mod && mod->OnChat(message))
			{
				return 1;
			}
		}

		return 0;
	}

	/* Function hook that gets called every game tick.
	 * @param	{cube::Game*} game
	 * @return	{void}
	*/
	virtual void OnGameTick(cube::Game* game) override {
		static bool greeted = false;
		if (!greeted && game && game->GetPlayer()) {
			greeted = true;
			std::wstring welcome = L"[CubeMegaMod] Mod carregado com sucesso!\n";
			game->PrintMessage(welcome.c_str(), 100, 255, 100);
		}

		for (CubeMod* mod : g_Mods) {
			if (mod) mod->OnGameTick(game);
		}

		for (const hook::HookEventData& e : hookEvents)
		{
			switch (e.type)
			{
			case hook::HookEvent::LoreInteraction:
				for (CubeMod* mod : g_Mods) {
					if (mod) mod->OnLoreIncrease(game, e.data);
				}
				break;
			default:
				break;
			}
		}
		hookEvents.clear();
	}

	virtual void OnGetKeyboardState(BYTE* diKeys) override {
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnGetKeyboardState(diKeys);
		}
	}

	/* Function hook that gets called on intialization of cubeworld.
	 * [Note]:	cube::GetGame() is not yet available here!!
	 * @return	{void}
	*/
	virtual void Initialize() override {
		g_Base = (char*)CWBase();
		modVector.push_back(new SeaExplorationMod());
		modVector.push_back(new LoreInteractionMod());
		modVector.push_back(new CombatUpdateMod());
		modVector.push_back(new CreatureUpdatesMod());
		modVector.push_back(new ShopUpdateMod());
		modVector.push_back(new WorldGenMod());
		modVector.push_back(new BeginnerModeMod());
		modVector.push_back(new RegionLockUpdateMod());
		modVector.push_back(new WeaponUpgradeMod());
		modVector.push_back(new QuestMod());
		modVector.push_back(new PlayerUpdatesMod());
		modVector.push_back(new StackUpdatesMod());

		cube::ApplySettings(&modVector);
		cube::SaveSettings(&modVector);

		// Add enabled mods to the global modlist.
		for (size_t i = 0; i < modVector.size(); i++)
		{
			if (modVector.at(i) && modVector.at(i)->m_Enabled)
			{
				g_Mods.push_back(modVector.at(i));
			}
		}

		// Setup handlers
		SetupChestInteractionHandler();
		SetupShopInteractionHandler();
		SetupOnCreatureDeathHandler();
		SetupItemPriceHandler();
		IncreaseLoreInitialize(&hookEvents); // Todo: Rename handler
		ItemDropPatchInitialize();
		SetupCreatureInteraction();

		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->Initialize();
		}
	}

	virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureArmorCalculated(creature, armor);
		}
	}

	virtual void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureCriticalCalculated(creature, critical);
		}
	}

	virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureAttackPowerCalculated(creature, power);
		}
	}

	virtual void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureSpellPowerCalculated(creature, power);
		}
	}

	virtual void OnCreatureHasteCalculated(cube::Creature* creature, float* haste) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureHasteCalculated(creature, haste);
		}
	}

	virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureHPCalculated(creature, hp);
		}
	}

	virtual void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureResistanceCalculated(creature, resistance);
		}
	}

	virtual void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureRegenerationCalculated(creature, regeneration);
		}
	}

	virtual void OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration) override
	{
		for (CubeMod* mod : g_Mods)
		{
			if (mod) mod->OnCreatureManaGenerationCalculated(creature, manaGeneration);
		}
	}
};

// Export of the mod created in this file, so that the modloader can see and use it.
EXPORT Mod* MakeMod() {
	return new Mod();
}