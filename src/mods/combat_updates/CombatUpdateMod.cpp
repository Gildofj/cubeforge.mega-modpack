#include "CombatUpdateMod.h"
#include "abilities/ConvertMTSAbility.h"
#include "abilities/HealAbility.h"
#include "abilities/FarJumpAbility.h"
#include "utils/GameHelper.h"
#include <cwchar>

CombatUpdateMod::CombatUpdateMod()
	: BaseMod("Combat Update Mod", "cubeforge-combat-updates", 3, { 1, 0, 0 })
{
	m_Data = { false };
	Load(&m_Data, sizeof(m_Data));
}

bool CombatUpdateMod::CheckMovementButtonPress(cube::DButton* button, cube::DButton* lCntr)
{
	if (button->Pressed() == cube::DButton::State::DoubleTap && m_Data.doubleTap)
	{
		return true;
	}

	if (!m_Data.doubleTap && button->Pressed() != cube::DButton::State::None && lCntr->Pressed() == cube::DButton::State::Held)
	{
		return true;
	}

	return false;
}

void CombatUpdateMod::OnGetKeyboardState(BYTE* diKeys)
{
	static cube::DButton KeyW(17); // W
	static cube::DButton KeyA(30); // A
	static cube::DButton KeyS(31); // S
	static cube::DButton KeyD(32); // D

	static cube::DButton KeyLCntrl(29); // LCtrl

	static cube::DButton Key1(2); // 1
	static cube::DButton Key2(3); // 2

	cube::Game* game = cube::GetGame();
	if (!game || cube::Helper::InGUI(game))
	{
		return;
	}

	KeyW.Update(diKeys);
	KeyA.Update(diKeys);
	KeyS.Update(diKeys);
	KeyD.Update(diKeys);

	KeyLCntrl.Update(diKeys);

	Key1.Update(diKeys);
	Key2.Update(diKeys);

	cube::Creature* player = game->GetPlayer();
	if (!player)
	{
		return;
	}

	if (Key1.Pressed() == cube::DButton::State::Pressed)
	{
		cube::ConvertMTSAbility().Execute(player);
	}
	if (Key2.Pressed() == cube::DButton::State::Pressed)
	{
		cube::HealAbility().Execute(player);
	}

	if (CheckMovementButtonPress(&KeyW, &KeyLCntrl))
	{
		cube::FarJumpAbility(0).Execute(player);
	}

	if (CheckMovementButtonPress(&KeyA, &KeyLCntrl))
	{
		cube::FarJumpAbility(1).Execute(player);
	}

	if (CheckMovementButtonPress(&KeyS, &KeyLCntrl))
	{
		cube::FarJumpAbility(3).Execute(player);
	}

	if (CheckMovementButtonPress(&KeyD, &KeyLCntrl))
	{
		cube::FarJumpAbility(2).Execute(player);
	}
}

int CombatUpdateMod::OnChat(std::wstring* message)
{
	if (!message) return 0;
	const wchar_t* msg = message->c_str();

	int val;
	if (swscanf_s(msg, L"/cubeforge combat doubletap %d", &val) == 1)
	{
		m_Data.doubleTap = (val != 0);
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(m_Data.doubleTap ? L"[CombatUpdates] Double-tap ativado.\n" : L"[CombatUpdates] Double-tap desativado (use LCtrl+WASD).\n", 100, 255, 100);
		return 1;
	}

	if (!wcscmp(msg, L"/enable doubletap"))
	{
		m_Data.doubleTap = true;
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(L"[CombatUpdates] Double-tap ativado.\n", 100, 255, 100);
		return 1;
	}

	if (!wcscmp(msg, L"/disable doubletap"))
	{
		m_Data.doubleTap = false;
		Save(&m_Data, sizeof(m_Data));
		cube::GetGame()->PrintMessage(L"[CombatUpdates] Double-tap desativado.\n", 100, 255, 100);
		return 1;
	}

	return 0;
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new CombatUpdateMod();
}
#endif
