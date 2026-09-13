#pragma once

#include "BaseMod.h"

class RegionLockMod : public BaseMod
{
public:
	RegionLockMod();
	virtual ~RegionLockMod() override = default;

	void Initialize() override;
};
