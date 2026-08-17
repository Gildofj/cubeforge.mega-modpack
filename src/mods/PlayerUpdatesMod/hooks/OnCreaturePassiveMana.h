/*
* Hook for modifying the ultimate ability ID.
*/
#pragma once

#include "cwsdk.h"
#include "../PlayerUpdatesMod.h"

extern "C" int OnCreatureManaModification(cube::Creature* player) {
	if (player->entity_data.classType <= 4)
	{
		return 0;
	}

	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= MOD->m_Classes.size())
	{
		return 0;
	}

	return MOD->m_Classes.at(index)->ManaGenerationType(player);
}

GETTER_VAR(void*, ASMOnManaHandler_jmpback);
extern "C" void ASMOnManaHandler();

void InitializeOnCreatureManaHandler()
{
	WriteFarJMP(CWOffset(0x2DFDBC), (void*)&ASMOnManaHandler);
	ASMOnManaHandler_jmpback = CWOffset(0x2DFDD2);
}