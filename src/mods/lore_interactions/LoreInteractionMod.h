#pragma once

#include "BaseMod.h"

class LoreInteractionMod : public BaseMod
{
public:
	LoreInteractionMod();
	virtual ~LoreInteractionMod() override;

	void Initialize() override;
	void OnLoreIncrease(cube::Game* game, int value) override;
};
