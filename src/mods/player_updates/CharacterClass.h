#pragma once
#include <string>
#include "cwsdk.h"

class CharacterClass
{
public:
	const char* m_Name;
	const wchar_t* m_WideName;
	int m_Id;

	enum ManaGenerationType : int
	{
		PassiveManaLoss = 0,
		PassiveManaGain,
		NoManaChange
	};

	enum Ability : int
	{
		FullHeal = 166,
		PuddleHeal = 167,
		PuddleLava = 168,
		PuddlePoison = 169,
		AnimationTest = 170,
	};

	CharacterClass(const char* name, const wchar_t* widename) : m_Name(name), m_WideName(widename), m_Id(0) {}
	virtual ~CharacterClass() = default;
	
	inline virtual void GenerateStarterGear(cube::Game* game, cube::Creature* player) {}
	inline virtual int GetUltimateAbilityId(cube::Creature* player) { return 0; }
	inline virtual int GetUltimateAbilityCooldown(cube::Creature* player, int abilityID) { return 0; }
	inline virtual int GetShiftAbilityId(cube::Creature* player) { return 0; }
	inline virtual int GetMiddleMouseAbilityId(cube::Creature * player) { return 0; }
	inline virtual bool CanEquipItem(cube::Item* item) { return true; }
	inline virtual int ManaGenerationType(cube::Creature* player) { return 0; }
	inline virtual int GetTreasureClassType(cube::Creature* player) { return 0; }
	inline virtual void Initialize(cube::Game* game, int id) {}

	static inline void IntializeAbilities(cube::Game* game)
	{
		if (!game) return;
		std::wstring skillCrush(L"SkillCrush");
		std::wstring skillBullwark(L"SkillBulwark");
		game->speech.skill_type_id_map.insert_or_assign(100, skillCrush);	
		game->speech.skill_type_id_map.insert_or_assign(166, skillBullwark);
		game->speech.skill_type_id_map.insert_or_assign(Ability::FullHeal, skillBullwark);
		game->speech.skill_type_id_map.insert_or_assign(Ability::PuddleHeal, skillBullwark);
		game->speech.skill_type_id_map.insert_or_assign(Ability::PuddleLava, skillBullwark);
		game->speech.skill_type_id_map.insert_or_assign(Ability::PuddlePoison, skillBullwark);
	}
};
