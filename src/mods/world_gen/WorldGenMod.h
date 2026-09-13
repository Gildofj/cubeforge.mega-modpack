#pragma once

#include "BaseMod.h"

class WorldGenMod : public BaseMod
{
public:
	WorldGenMod();
	virtual ~WorldGenMod() override = default;

	void Initialize() override;
};
