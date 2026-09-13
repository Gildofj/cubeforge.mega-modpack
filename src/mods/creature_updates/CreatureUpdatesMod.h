#pragma once

#include "BaseMod.h"

class CreatureUpdatesMod : public BaseMod
{
public:
	CreatureUpdatesMod();
	virtual ~CreatureUpdatesMod() override = default;

	void Initialize() override;

	void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override;
	void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) override;
	void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureHasteCalculated(cube::Creature* creature, float* haste) override;
	void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override;
	void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override;
	void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) override;
	void OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration) override;
};
