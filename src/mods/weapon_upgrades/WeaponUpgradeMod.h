#pragma once

#include "BaseMod.h"

class WeaponUpgradeMod : public BaseMod
{
public:
	WeaponUpgradeMod();
	virtual ~WeaponUpgradeMod() override = default;

	void Initialize() override;
	int OnCreatureTalk(cube::Game* game, cube::Creature* creature) override;
};
