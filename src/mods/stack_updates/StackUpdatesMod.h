#pragma once

#include "BaseMod.h"

class StackUpdatesMod : public BaseMod
{
public:
	StackUpdatesMod();
	virtual ~StackUpdatesMod() override = default;

	void Initialize() override;
};
