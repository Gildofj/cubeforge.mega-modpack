#include "CreatureUpdatesMod.h"
#include "creature/Creature.h"
#include "memory/MemoryHelper.h"

static void BuffPet(cube::Creature* creature, float* stat)
{
	if (!creature || !stat) return;
	if (creature->entity_data.hostility_type == static_cast<char>(cube::Enums::EntityBehaviour::Pet))
	{
		*stat *= 1.5f;
	}
}

CreatureUpdatesMod::CreatureUpdatesMod()
	: BaseMod("Creature Updates Mod", "cubeforge-creature-updates", 4, { 1, 0, 0 })
{
}

void CreatureUpdatesMod::Initialize()
{
	// 40 Gold inicial: mov eax, 0x28; nop
	auto offset = 0x83590;
	unsigned char data[] = { 0xB8, 0x28, 0x00, 0x00, 0x00, 0x90 };
	MemoryHelper::PatchMemory((void*)CWOffset(offset), data, sizeof(data));
}

void CreatureUpdatesMod::OnCreatureArmorCalculated(cube::Creature* creature, float* armor)
{
	BuffPet(creature, armor);
}

void CreatureUpdatesMod::OnCreatureCriticalCalculated(cube::Creature* creature, float* critical)
{
	BuffPet(creature, critical);
}

void CreatureUpdatesMod::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power)
{
	if (!creature || !power) return;

	if (creature->entity_data.classType == static_cast<int>(cube::Enums::ClassType::Ranger) &&
		creature->entity_data.hostility_type == static_cast<int>(cube::Enums::EntityBehaviour::Hostile))
	{
		if (creature->entity_data.equipment.weapon_right.id == 8 ||
			creature->entity_data.equipment.weapon_left.id == 8)
		{
			*power *= 0.5f;
		}
	}

	BuffPet(creature, power);
}

void CreatureUpdatesMod::OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power)
{
	if (!creature || !power) return;

	if (creature->entity_data.classType == static_cast<int>(cube::Enums::ClassType::Mage) &&
		creature->entity_data.hostility_type == static_cast<int>(cube::Enums::EntityBehaviour::Hostile))
	{
		*power *= 0.5f;
	}

	BuffPet(creature, power);
}

void CreatureUpdatesMod::OnCreatureHasteCalculated(cube::Creature* creature, float* haste)
{
	BuffPet(creature, haste);
}

void CreatureUpdatesMod::OnCreatureHPCalculated(cube::Creature* creature, float* hp)
{
	BuffPet(creature, hp);
}

void CreatureUpdatesMod::OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance)
{
	BuffPet(creature, resistance);
}

void CreatureUpdatesMod::OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration)
{
	BuffPet(creature, regeneration);
}

void CreatureUpdatesMod::OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration)
{
	BuffPet(creature, manaGeneration);
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new CreatureUpdatesMod();
}
#endif
