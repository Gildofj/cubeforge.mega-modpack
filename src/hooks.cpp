#include "hooks.h"
#include "CubeMod.h"
#include <cmath>
#include <cwchar>

#include "hooks/ChestInteractionHandler.h"
#include "hooks/ShopInteractionHandler.h"
#include "hooks/ItemPriceHandler.h"
#include "hooks/lore_increase.h"
#include "hooks/ItemDropPatch.h"
#include "hooks/CreatureDeathHandler.h"
#include "hooks/CreatureTalkHandler.h"

// ==============================================================================
// Global Variable Definitions for MASM Hooks
// ==============================================================================
extern "C" {
	void* ASMIsChestOpen_jmpback = nullptr;
	void* ASMIsChestOpen_bail = nullptr;
	void* ASMChestInteractionHandler_jmpback = nullptr;
	void* ASMOnCreatureDeath_jmpback = nullptr;
	void* ASMOnTalkToCreature_jmpback = nullptr;
	void* ASMOnTalkToCreature_jmpback2 = nullptr;
	void* ASMOnTalkToCreature_bail = nullptr;
	void* ASMPatchItemDrop_jmpback = nullptr;
	void* ASMItemPrice_jmpback = nullptr;
	void* ASMIncreaseLore_jmpback = nullptr;
	void* ASMShopInteraction_jmpback = nullptr;
	void* ASMSetMenuClassName_retn = nullptr;
	void* ASMSetClassName_1_ret = nullptr;
	void* ASMSetClassName_2_ret = nullptr;
	void* OverwriteClassIdIncrease_ret = nullptr;
	void* OverwriteClassIdDecrease_ret = nullptr;
	void* AddCreatureStarterWeapons_ret = nullptr;
	void* ASMOnCanEquipItem_jmpback = nullptr;
	void* ASMOnCanEquipItem_jmpback_2 = nullptr;
	void* ASMOnCanEquipItem_bail = nullptr;
	void* ASMOnManaHandler_jmpback = nullptr;
	void* ASMOnExecuteAbility_jmpback = nullptr;
	void* ASMOnExecuteAbility_jmpback_2 = nullptr;
	void* ASMOnDrawColorPicker_jmpback = nullptr;
	void* ASMOnDrawColorPicker_bail = nullptr;
	void* ASMOnDrawHairColor_jmpback = nullptr;
	void* ASMOnDrawHairCut_jmpback = nullptr;
	void* ASMOnDrawFace_jmpback = nullptr;
	void* ASMOnDrawGender_jmpback = nullptr;
	void* ASMOnGenerateCreature_jmpback = nullptr;
	void* ASMOnGetRaceName_jmpback = nullptr;
	void* ASMOnGetRaceName2_jmpback = nullptr;
	void* ASMOnGetShiftAbilityID_jmpback = nullptr;
	void* ASMOnGetShiftAbilityID_jmpback_2 = nullptr;
	void* ASMTreasureFixHandler_ret_1 = nullptr;
	void* ASMTreasureFixHandler_ret_2 = nullptr;
	void* ASMTreasureFixHandler_jmpback = nullptr;
	void* ASMOnGetItemDescription_jmpback = nullptr;
	void* ASMOnGetItemDescription_jmpback_2 = nullptr;
	void* ASMOnGetItemDescription_bail = nullptr;
	void* ASMOnGetItemName_jmpback = nullptr;
	void* ASMOnGetItemName_bail = nullptr;
	void* ASMWeaponUpgradePreview_jmpback = nullptr;
}

// ==============================================================================
// Hook Functions Called from MASM
// ==============================================================================

// --- Chest Interaction ---
extern "C" void OnChestInteraction(cube::Game* game, cube::Creature* creature) {
	if (!game || !creature)
	{
		return;
	}

	int type = creature->entity_data.race - 181;
	if (!ChestInteractionHandler(game, creature, type))
	{
		return;
	}

	// Check if the chest is visible on the map.
	unsigned long long mask = (1ULL << (int)cube::Enums::StateFlags::VisibleOnMap);
	if ((creature->entity_data.binary_toggles & mask) != 0)
	{
		creature->entity_data.binary_toggles &= ~mask;
	}

	// Open the chest
	creature->entity_data.interaction_state = 2;
}

// --- Creature Death ---
extern "C" void OnCreatureDeath(cube::Creature * creature, cube::Creature * attacker) {
	if (attacker == nullptr)
	{
		attacker = cube::GetGame()->GetPlayer();
	}
	for (CubeMod* mod : g_Mods) {
		mod->OnCreatureDeath(creature, attacker);
	}
}

// --- Creature Talk ---
extern "C" int OnTalkToCreature(cube::Game * game, cube::Creature * creature)
{
	for (CubeMod* mod : g_Mods)
	{
		if (mod->OnCreatureTalk(game, creature))
		{
			return 1;
		}
	}
	return 0;
}

// --- Item Drop ---
extern "C" IntVector2* GetDropRegion(void) {
	return GetPlayerRegion();
}

// --- Item Price ---
extern "C" void OnItemPriceHandler(cube::Item* item, int* price) {
	if (!item || !price)
	{
		return;
	}

	switch (item->category)
	{
	case 1:
		if (item->id == 1)
		{
			*price = 5 * (item->rarity + 1);
		}
		break;
	case 11:
		if (item->id == 14)
		{
			*price = 25;
		}
		break;
	case 23:
		*price = 1000;
		break;
	case 24:
		if (item->id > 13 && item->id < 18)
		{
			*price = 25 * (item->id - 13);
		}
		else if (item->id == 12)
		{
			*price = 25;
		}
		else
		{
			*price = 75;
		}
		break;
	default:
		break;
	}
}

extern "C" float callPowf(float a1, float a2)
{
	return std::pow(a1, a2);
}

// --- Lore Increase ---
extern "C" void OnPrintRGBA(std::wstring* message) {
	if (!message) return;
	const wchar_t* msg = message->c_str();
	int index = 0;

	if (g_hookEvents && swscanf_s(msg, L" lore increased to %d", &index) == 1)
	{
		g_hookEvents->push_back({ hook::HookEvent::LoreInteraction,  index });
	}
}

// --- Shop Interaction ---
extern "C" void OnShopInteractionHandler(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id) {
	std::vector<std::vector<cube::ItemStack >> temp;
	temp.push_back(std::vector<cube::ItemStack>());

	bool clear = false;
	for (CubeMod* mod : g_Mods) {
		if (mod->OnShopInteraction(game, &temp, classType, id)) {
			clear = true;
		}
	}

	if (clear)
	{
		itemVector->at(0).clear();
	}

	for (auto i : temp.at(0))
	{
		itemVector->at(0).push_back(i);
	}
}

extern "C" void ShopInteractionSub1(void* p1, void* p2)
{
	((void (*)(void*, void*))CWOffset(0x64F30))(p1, p2);
}

extern "C" void ShopInteractionSub2(void* p1)
{
	((void (*)(void*))CWOffset(0x280A40))(p1);
}

void hook::InitializeAll(std::vector<HookEventData>* hookEvents)
{
}
