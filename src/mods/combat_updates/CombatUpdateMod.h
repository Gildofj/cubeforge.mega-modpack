#pragma once

#include "BaseMod.h"
#include "input/DButton.h"

struct CombatUpdateData
{
	bool doubleTap;
};

class CombatUpdateMod : public BaseMod
{
private:
	CombatUpdateData m_Data;

	bool CheckMovementButtonPress(cube::DButton* button, cube::DButton* lCntr);

public:
	CombatUpdateMod();
	virtual ~CombatUpdateMod() override = default;

	void OnGetKeyboardState(BYTE* diKeys) override;
	int OnChat(std::wstring* message) override;
};
