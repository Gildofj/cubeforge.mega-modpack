#include "BeginnerModeMod.h"
#include "creature/Creature.h"

static void ApplyBeginnerBuffs(cube::Creature* creature, float* stat)
{
	if (!creature || !stat) return;

	if (creature->entity_data.hostility_type != static_cast<char>(cube::Enums::EntityBehaviour::Hostile))
	{
		return;
	}

	cube::Game* game = cube::GetGame();
	if (!game) return;

	cube::Creature* player = game->GetPlayer();
	if (!player) return;

	int level = player->entity_data.level + 1;
	if (level > 5)
	{
		return;
	}

	*stat *= (level / 6.f);
}

BeginnerModeMod::BeginnerModeMod()
	: BaseMod("Beginner Mode Mod", "cubeforge-beginner-mode", 7, { 1, 0, 0 })
{
}

void BeginnerModeMod::OnCreatureArmorCalculated(cube::Creature* creature, float* armor)
{
	ApplyBeginnerBuffs(creature, armor);
}

void BeginnerModeMod::OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power)
{
	ApplyBeginnerBuffs(creature, power);
}

void BeginnerModeMod::OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power)
{
	ApplyBeginnerBuffs(creature, power);
}

void BeginnerModeMod::OnCreatureHPCalculated(cube::Creature* creature, float* hp)
{
	ApplyBeginnerBuffs(creature, hp);
}

void BeginnerModeMod::OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance)
{
	ApplyBeginnerBuffs(creature, resistance);
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new BeginnerModeMod();
}
#endif
