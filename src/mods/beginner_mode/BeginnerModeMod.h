#pragma once

#include "BaseMod.h"

class BeginnerModeMod : public BaseMod
{
public:
	BeginnerModeMod();
	virtual ~BeginnerModeMod() override = default;

	void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override;
	void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override;
	void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override;
};
