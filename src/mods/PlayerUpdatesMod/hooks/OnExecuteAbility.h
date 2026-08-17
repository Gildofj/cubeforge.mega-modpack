/*
* Hook for executing abilities.
*/
#pragma once

#include "cwsdk.h"
#include "../PlayerUpdatesMod.h"

#include "../abilities/FullHealAbility.h"
#include "../abilities/HealingPuddle.h"
#include "../abilities/LavaPuddle.h"
#include "../abilities/PoisonPuddle.h"

void AnimationTest(cube::World* world, cube::Creature* player);

extern "C" void OnExecuteAbility(cube::World* world, cube::Creature* player) {
	if (player->entity_data.classType <= 4 || player->entity_data.current_ability <= 165)
	{
		return;
	}

	switch (player->entity_data.current_ability)
	{
	case CharacterClass::Ability::FullHeal:
		cube::GetGame()->PrintMessage(L"Execute Bulwark full heal!");
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
	case CharacterClass::Ability::AnimationTest:
		AnimationTest(world, player);
		break;
	default:
		break;
	}
}

void AnimationTest(cube::World* world, cube::Creature* player)
{
	float time = player->entity_data.time_since_ability;
	int max_time = 500.f;

	if (time > max_time)
	{
		player->entity_data.current_ability = 0;
	}

	player->entity_data.appearance.chest_rotation = (time / max_time) * 360.f;
}

GETTER_VAR(void*, ASMOnExecuteAbility_jmpback);
GETTER_VAR(void*, ASMOnExecuteAbility_jmpback_2);
extern "C" void ASMOnExecuteAbility();

void InitializeOnExecuteAbilityHandler()
{
	WriteFarJMP(CWOffset(0x2BBAB2), (void*)&ASMOnExecuteAbility);
	ASMOnExecuteAbility_jmpback = CWOffset(0x2BBAC3);
	ASMOnExecuteAbility_jmpback_2 = CWOffset(0x2BBAF4);
	// Todo: Bail
	//ASMOnExecuteAbility_bail = CWOffset(0x109677);
}