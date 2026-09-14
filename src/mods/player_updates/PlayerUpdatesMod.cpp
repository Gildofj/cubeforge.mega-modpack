#include "PlayerUpdatesMod.h"
#include "classes/MonkClass.h"
#include "abilities/FullHealAbility.h"
#include "abilities/HealingPuddle.h"
#include "abilities/LavaPuddle.h"
#include "abilities/PoisonPuddle.h"
#include "creature/Creature.h"
#include "memory/MemoryHelper.h"
#include <ctime>
#include <cstdlib>

static PlayerUpdatesMod* MOD = nullptr;

const static int STANDARD_CLASS_COUNT = 4;
static const char* CLASSNAME_ROGUE = "Rogue";
static const wchar_t* CLASSNAME_ROGUE_WIDE = L"Rogue";

// Race definition
struct RaceData
{
	int race_type;
	bool gender, face, haircut, hair_color;
};

const static RaceData RACES[] = {
	{ 79, false, false, false, false },	// 8: Jester
	{ 80, false, false, true, true },	// 9: Spectrino
	{ 91, false, false, false, false },	// 10: Raccoon
	{ 97, false, false, false, true },	// 11: Vampire
	{ 165, false, true, false, true },	// 12: Skeleton Knight
	{ 253, false, false, false, false },	// 13: Archdruid
	{ 254, false, false, false, false },	// 14: Half-demon
};

enum ControlIndex : int
{
	button_attack = 0,
	button_special_attack,
	button_class_skill,
	button_ultimate_skill,
	button_quick_item,
	button_up,
	button_down,
	button_left,
	button_right,
	button_jump,
	button_dodge,
	button_use,
	button_lamp,
	button_walk,
	button_call_pet,
	button_world_map,
	button_ring_menu,
	button_inventory,
	button_crafting_menu,
	button_system_menu,
	button_multiplayer_menu,
	button_zoom_in,
	button_zoom_out
};

// Global variables for MASM jumps
extern "C" {
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

	void* ASMTreasureFixHandler_jmpback = nullptr;
	void* ASMTreasureFixHandler_ret_1 = nullptr;
	void* ASMTreasureFixHandler_ret_2 = nullptr;

	void ASMSetMenuClassName();
	void ASMSetClassName_1();
	void ASMSetClassName_2();
	void ASMMenuClassIdIncrease();
	void ASMMenuClassIdDecrease();
	void ASMAddCreatureStarterWeapons();
	void ASMOnCanEquipItem();
	void ASMOnManaHandler();
	void ASMOnExecuteAbility();
	void ASMOnDrawColorPicker();
	void ASMOnDrawHairColor();
	void ASMOnDrawHairCut();
	void ASMOnDrawFace();
	void ASMOnDrawGender();
	void ASMOnGenerateCreature();
	void ASMOnGetRaceName();
	void ASMOnGetRaceName2();
	void ASMOnGetShiftAbilityID();
	void ASMOnGetUltimateAbilityCooldown();
	void ASMOnGetUltimateAbilityID();
	void ASMTreasureFixHandler();
}

static std::vector<int> s_CooldownMap(256, 0);

static void SetCooldown(const std::vector<int>& ids, int cooldown) {
	for (size_t i = 0; i < ids.size(); i++) {
		s_CooldownMap.at(ids[i]) = cooldown;
	}
}

static void SetupCooldowns() {
	SetCooldown({ 51, 55, 115 }, 16000);
	SetCooldown({ 47, 109, 112, 113, 114, 142, 144, 146, 165 }, 40000);
	SetCooldown({ 95, 98, 136, 149 }, 30000);
	SetCooldown({ 22, 52, 97 }, 12000);
	SetCooldown({ 111 }, 4000);
	SetCooldown({ 73 }, 15000);
	SetCooldown({ 140, 141, 143 }, 60000);
	SetCooldown({ 148 }, 20000);
	SetCooldown({ 50, 108 }, 16000);
}

// C/C++ implementations called from ASM
extern "C" const wchar_t* GetCubeClassNameWide(int id) {
	int index = id - 1;
	if (!MOD || index < 0 || index >= static_cast<int>(MOD->m_Classes.size()))
	{
		return CLASSNAME_ROGUE_WIDE;
	}
	return MOD->m_Classes.at(index)->m_WideName;
}

extern "C" const char* GetCubeClassName(int id) {
	int index = id - 1;
	if (!MOD || index < 0 || index >= static_cast<int>(MOD->m_Classes.size()))
	{
		return CLASSNAME_ROGUE;
	}
	return MOD->m_Classes.at(index)->m_Name;
}

extern "C" void OnCharacterWidgetClassButtonIncrease(cube::CharacterStyleWidget* widget) {
	if (!widget || !MOD) return;
	widget->class_type++;
	if (widget->class_type >= STANDARD_CLASS_COUNT + static_cast<int>(MOD->m_Classes.size()))
	{
		widget->class_type = 0;
	}
}

extern "C" void OnCharacterWidgetClassButtonDecrease(cube::CharacterStyleWidget* widget) {
	if (!widget || !MOD) return;
	widget->class_type--;
	if (widget->class_type < 0)
	{
		widget->class_type = STANDARD_CLASS_COUNT + static_cast<int>(MOD->m_Classes.size()) - 1;
	}
}

extern "C" char GenerateStarterGear(cube::Game* game) {
	if (!game || !MOD) return 0;
	cube::Creature* player = game->GetPlayer();
	if (!player) return 0;

	if (player->entity_data.classType <= STANDARD_CLASS_COUNT)
	{
		return game->GeneratePlayerStarterGear();
	}

	int index = player->entity_data.classType - STANDARD_CLASS_COUNT - 1;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size()))
	{
		return 0;
	}

	player->ClearInventoryGear();
	player->entity_data.equipment.Clear();
	MOD->m_Classes.at(index)->GenerateStarterGear(game, player);
	return 0;
}

extern "C" int OnCanEquipItem(cube::Item* item, int classType) {
	if (!MOD || classType <= 4)
	{
		return 0;
	}

	int index = classType - 5;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size()))
	{
		return 0;
	}

	return MOD->m_Classes.at(index)->CanEquipItem(item);
}

extern "C" int OnCreatureManaModification(cube::Creature* player) {
	if (!MOD || !player || player->entity_data.classType <= 4)
	{
		return 0;
	}

	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size()))
	{
		return 0;
	}

	return MOD->m_Classes.at(index)->ManaGenerationType(player);
}

extern "C" void OnExecuteAbility(cube::World* world, cube::Creature* player) {
	if (!player || player->entity_data.classType <= 4 || player->entity_data.current_ability <= 165)
	{
		return;
	}

	switch (player->entity_data.current_ability)
	{
	case CharacterClass::Ability::FullHeal:
		ExecuteFullHealAbility(player, true);
		break;
	case CharacterClass::Ability::PuddleHeal:
		ExecuteSelfHealingPuddleAbility(world, player);
		player->entity_data.hit_combo += 20;
		player->entity_data.time_since_hit = 0;
		break;
	case CharacterClass::Ability::PuddleLava:
		player->entity_data.current_ability = 0;
		if (player->entity_data.MP >= 0.2f)
		{
			ExecuteSelfLavaPuddleAbility(world, player);
			player->entity_data.MP -= 0.1f;
			player->entity_data.hit_combo += 1;
			player->entity_data.time_since_hit = 0;
		}
		break;
	case CharacterClass::Ability::PuddlePoison:
		player->entity_data.current_ability = 0;
		if (player->stamina >= 0.5)
		{
			ExecuteSelfPoisonPuddleAbility(world, player);
			player->stamina -= 0.5f;
			player->entity_data.hit_combo += 5;
			player->entity_data.time_since_hit = 0;
		}
		break;
	default:
		break;
	}
}

static bool SetPreAppearance(cube::SaveData* saveData, cube::Creature::EntityData::Appearance* appearance)
{
	if (!saveData || !appearance) return false;
	appearance->hair_color = saveData->hair_color;
	if (saveData->race == 9 && saveData->haircut > 0)
	{
		appearance->hair_model = static_cast<short>(1489 + saveData->haircut);
	}
	return false;
}

extern "C" void OnGenerateCreature(int* race, cube::SaveData* save_data, cube::Creature::EntityData::Appearance* appearance) {
	if (!race || !save_data) return;
	int gender = save_data->gender;
	int calc_race = 0;

	switch (save_data->race)
	{
	case 0: calc_race = (gender & 1) + 2; break;
	case 1: calc_race = gender & 1; break;
	case 2: calc_race = (gender & 1) + 9; break;
	case 3: calc_race = (gender & 1) + 11; break;
	case 4: calc_race = (gender & 1) + 4; break;
	case 5: calc_race = (gender & 1) + 7; break;
	case 6: calc_race = (gender & 1) + 15; break;
	case 7: calc_race = (gender & 1) + 13; break;
	default:
	{
		int index = save_data->race - 8;
		int size = sizeof(RACES) / sizeof(RACES[0]);
		if (index >= 0 && index < size) {
			calc_race = RACES[index].race_type;
		}
		break;
	}
	}

	*race = calc_race;
	if (SetPreAppearance(save_data, appearance))
	{
		*race = -1;
	}
}

static std::string* GetRaceNameInternal(std::string* string, int race)
{
	switch (race)
	{
	case 2: case 3: *string = "Human"; break;
	case 0: case 1: *string = "Elf"; break;
	case 11: case 12: *string = "Orc"; break;
	case 9: case 10: *string = "Dwarf"; break;
	case 4: case 5: *string = "Goblin"; break;
	case 7: case 8: *string = "Lizard"; break;
	case 15: case 16: *string = "Undead"; break;
	case 13: case 14: *string = "Frog"; break;
	case 95: *string = "Santa"; break;
	case 83: case 84: *string = "Nomad"; break;
	default:
	{
		cube::Game* game = cube::GetGame();
		if (game != nullptr)
		{
			auto res = game->speech.entity_type_id_map.find(race);
			if (res != game->speech.entity_type_id_map.end())
			{
				std::wstring* wstr = &res->second;
				string->clear();
				string->reserve(wstr->length());
				for (wchar_t wc : *wstr)
				{
					string->push_back(static_cast<char>(wc));
				}
			}
		}
		break;
	}
	}
	return string;
}

extern "C" void OnGetRaceName(void* toAppend, cube::Creature* creature)
{
	if (!creature) return;
	std::string name;
	GetRaceNameInternal(&name, creature->entity_data.race);
	((void (*)(void*, const char*))CWOffset(0x6D8E0))(toAppend, name.c_str());
}

extern "C" void OnGetRaceName2(void* toAppend, int raceIndex)
{
	std::string name;
	int race = 0;
	cube::SaveData data;
	data.race = raceIndex;
	data.gender = 0;
	OnGenerateCreature(&race, &data, nullptr);
	GetRaceNameInternal(&name, race);
	std::wstring wname(name.begin(), name.end());
	((void (*)(void*, const wchar_t*))CWOffset(0x486B0))(toAppend, wname.c_str());
}

extern "C" void OnGetGenderText(void* toAppend, cube::CharacterStyleWidget* widget)
{
	if (!widget) return;
	std::string name = (widget->gender == 1) ? "Female" : "Male";
	if (widget->race > 7)
	{
		int index = widget->race - 8;
		if (index >= 0 && index < sizeof(RACES) / sizeof(RACES[0]) && !RACES[index].gender)
		{
			name = "<None>";
		}
	}
	((void (*)(void*, const char*))CWOffset(0x6D8E0))(toAppend, name.c_str());
}

extern "C" void OnGetFaceText(void* toAppend, cube::CharacterStyleWidget* widget)
{
	if (!widget) return;
	std::wstring name = L"Face " + std::to_wstring(widget->face + 1);
	if (widget->race > 7)
	{
		int index = widget->race - 8;
		if (index >= 0 && index < sizeof(RACES) / sizeof(RACES[0]) && !RACES[index].face)
		{
			name = L"<None>";
		}
	}
	((void (*)(void*, const wchar_t*))CWOffset(0x486B0))(toAppend, name.c_str());
}

extern "C" void OnGetHairCutText(void* toAppend, cube::CharacterStyleWidget* widget)
{
	if (!widget) return;
	std::wstring name = L"Haircut " + std::to_wstring(widget->haircut + 1);
	if (widget->race > 7)
	{
		int index = widget->race - 8;
		if (index >= 0 && index < sizeof(RACES) / sizeof(RACES[0]) && !RACES[index].haircut)
		{
			name = L"<None>";
		}
	}
	((void (*)(void*, const wchar_t*))CWOffset(0x486B0))(toAppend, name.c_str());
}

extern "C" void OnGetHairColorText(void* toAppend, cube::CharacterStyleWidget* widget)
{
	if (!widget) return;
	std::wstring name = L"Hair color";
	if (widget->race > 7)
	{
		int index = widget->race - 8;
		if (index >= 0 && index < sizeof(RACES) / sizeof(RACES[0]))
		{
			RaceData data = RACES[index];
			if (data.hair_color && !data.haircut) name = L"Eye color";
			if (!data.hair_color) name = L"";
		}
	}
	((void (*)(void*, const wchar_t*))CWOffset(0x486B0))(toAppend, name.c_str());
}

extern "C" int OnGetHairColorDisplay(cube::CharacterStyleWidget* widget)
{
	if (!widget) return 0;
	if (widget->race > 7)
	{
		int index = widget->race - 8;
		if (index >= 0 && index < sizeof(RACES) / sizeof(RACES[0]) && !RACES[index].hair_color)
		{
			return 0;
		}
	}
	widget->preview_hair_color = widget->current_hair_color;
	return 1;
}

extern "C" int OnShiftAbilityID(cube::Game* game, cube::Creature* player)
{
	if (!game || !player || !MOD || player->entity_data.classType <= 4) return 0;
	if (player->entity_data.hostility_type != static_cast<int>(cube::Enums::EntityBehaviour::Player)) return 0;

	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size())) return 0;

	if (game->control_states[ControlIndex::button_class_skill])
	{
		player->entity_data.current_ability = static_cast<BYTE>(MOD->m_Classes.at(index)->GetShiftAbilityId(player));
	}

	if (game->control_states[ControlIndex::button_dodge])
	{
		int ability = MOD->m_Classes.at(index)->GetMiddleMouseAbilityId(player);
		player->entity_data.current_ability = static_cast<BYTE>(ability);
		if (ability > 0)
		{
			player->entity_data.time_since_ability = 0.f;
			game->control_states[ControlIndex::button_dodge] = false;
		}
	}
	return 0;
}

extern "C" int OnGetUltimateAbilityCooldown(cube::Creature* player, int abilityID)
{
	if (abilityID > 255 || !player) return 0;
	if (player->entity_data.classType <= 4) return s_CooldownMap.at(abilityID);

	if (!MOD) return 0;
	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size())) return 0;

	return MOD->m_Classes.at(index)->GetUltimateAbilityCooldown(player, abilityID);
}

extern "C" int OnGetUltimateAbilityID(cube::Creature* player)
{
	if (!player) return 0;
	switch (player->entity_data.classType)
	{
	case 1: return 148 + (player->entity_data.specialization > 0 ? 1 : 0);
	case 2: return 143 - (player->entity_data.specialization > 0 ? 1 : 0);
	case 3: return (player->entity_data.specialization == 0) ? 47 : 144;
	case 4: return (player->entity_data.specialization == 0) ? 109 : 141;
	default:
	{
		if (!MOD) return 0;
		int index = player->entity_data.classType - 5;
		if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size())) return 0;
		return MOD->m_Classes.at(index)->GetUltimateAbilityId(player);
	}
	}
}

extern "C" int GetTreasureDropClass(cube::Creature* player)
{
	if (!player) return 1;
	if (player->entity_data.classType <= 4) return player->entity_data.classType;

	if (!MOD) return (rand() % 4) + 1;
	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= static_cast<int>(MOD->m_Classes.size())) return (rand() % 4) + 1;

	int res = MOD->m_Classes.at(index)->GetTreasureClassType(player);
	return (res == 0) ? (rand() % 4) + 1 : res;
}

PlayerUpdatesMod::PlayerUpdatesMod()
	: BaseMod("Player Updates Mod", "cubeforge-player-updates", 11, { 1, 1, 0 })
{
	MOD = this;
}

PlayerUpdatesMod::~PlayerUpdatesMod()
{
	for (CharacterClass* c : m_Classes)
	{
		delete c;
	}
	m_Classes.clear();
	if (MOD == this)
	{
		MOD = nullptr;
	}
}

void PlayerUpdatesMod::Initialize()
{
	MOD = this;
	m_Classes.push_back(new MonkClass());

	// 1. Menu Hooks
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2714CF), (void*)&ASMSetMenuClassName);
	ASMSetMenuClassName_retn = (void*)CWOffset(0x2714F6);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x27405A), (void*)&ASMSetClassName_1);
	ASMSetClassName_1_ret = (void*)CWOffset(0x274081);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x26F5D2), (void*)&ASMSetClassName_2);
	ASMSetClassName_2_ret = (void*)CWOffset(0x26F5F9);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x27243A), (void*)&ASMMenuClassIdIncrease);
	OverwriteClassIdIncrease_ret = (void*)CWOffset(0x272459);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2723EA), (void*)&ASMMenuClassIdDecrease);
	OverwriteClassIdDecrease_ret = (void*)CWOffset(0x27240A);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x273176), (void*)&ASMAddCreatureStarterWeapons);
	AddCreatureStarterWeapons_ret = (void*)CWOffset(0x27318D);

	// 2. Ultimate Hooks
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x656E0), (void*)&ASMOnGetUltimateAbilityID);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x50BD0), (void*)&ASMOnGetUltimateAbilityCooldown);
	SetupCooldowns();

	// 3. Equip Hook
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x1094E9), (void*)&ASMOnCanEquipItem);
	ASMOnCanEquipItem_jmpback = (void*)CWOffset(0x1094F7);
	ASMOnCanEquipItem_jmpback_2 = (void*)CWOffset(0x1095CF);
	ASMOnCanEquipItem_bail = (void*)CWOffset(0x109677);

	// 4. Shift & Mana
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x8F223), (void*)&ASMOnGetShiftAbilityID);
	ASMOnGetShiftAbilityID_jmpback = (void*)CWOffset(0x8F237);
	ASMOnGetShiftAbilityID_jmpback_2 = (void*)CWOffset(0x8F240);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2DFDBC), (void*)&ASMOnManaHandler);
	ASMOnManaHandler_jmpback = (void*)CWOffset(0x2DFDD2);

	// 5. Treasure Fix & Abilities
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x10A7FE), (void*)&ASMTreasureFixHandler);
	ASMTreasureFixHandler_jmpback = (void*)CWOffset(0x10A810);
	ASMTreasureFixHandler_ret_1 = (void*)CWOffset(0x10AB70);
	ASMTreasureFixHandler_ret_2 = (void*)CWOffset(0x10AA60);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2BBAB2), (void*)&ASMOnExecuteAbility);
	ASMOnExecuteAbility_jmpback = (void*)CWOffset(0x2BBAC3);
	ASMOnExecuteAbility_jmpback_2 = (void*)CWOffset(0x2BBAF4);

	// 6. Creature Appearance & Races
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x53FDB), (void*)&ASMOnGenerateCreature);
	ASMOnGenerateCreature_jmpback = (void*)CWOffset(0x54057);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x26F548), (void*)&ASMOnGetRaceName);
	ASMOnGetRaceName_jmpback = (void*)CWOffset(0x26F5A4);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x271144), (void*)&ASMOnGetRaceName2);
	ASMOnGetRaceName2_jmpback = (void*)CWOffset(0x2711A5);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x27135A), (void*)&ASMOnDrawGender);
	ASMOnDrawGender_jmpback = (void*)CWOffset(0x27137B);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x271640), (void*)&ASMOnDrawFace);
	ASMOnDrawFace_jmpback = (void*)CWOffset(0x271663);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2717A3), (void*)&ASMOnDrawHairCut);
	ASMOnDrawHairCut_jmpback = (void*)CWOffset(0x2717C6);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x271D2A), (void*)&ASMOnDrawHairColor);
	ASMOnDrawHairColor_jmpback = (void*)CWOffset(0x271D3B);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x271DC2), (void*)&ASMOnDrawColorPicker);
	ASMOnDrawColorPicker_jmpback = (void*)CWOffset(0x271DE6);
	ASMOnDrawColorPicker_bail = (void*)CWOffset(0x272326);

	MemoryHelper::WriteByte((void*)CWOffset(0x272648 + 0x02), static_cast<unsigned char>(7 + sizeof(RACES) / sizeof(RACES[0])));
	MemoryHelper::WriteByte((void*)CWOffset(0x2725F1 + 0x01), static_cast<unsigned char>(7 + sizeof(RACES) / sizeof(RACES[0])));
}

void PlayerUpdatesMod::OnGameTick(cube::Game* game)
{
	static bool initialized = false;
	if (!initialized && game)
	{
		initialized = true;

		// Fix crafting tabs
		for (int i = 0; i < 6; i++)
		{
			MemoryHelper::WriteByte((void*)CWOffset(0x107721 + i), 0x90);
		}

		CharacterClass::IntializeAbilities(game);

		int i = 0;
		for (CharacterClass* characterClass : m_Classes)
		{
			if (characterClass)
			{
				characterClass->Initialize(game, i + 5);
				i++;
			}
		}
	}
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new PlayerUpdatesMod();
}
#endif
