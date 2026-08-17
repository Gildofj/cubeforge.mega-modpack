/*
* Hook for modifying the ultimate ability ID.
*/
#pragma once

#include "cwsdk.h"
#include "../PlayerUpdatesMod.h"

extern "C" int OnCanEquipItem(cube::Item* item, int classType) {
	if (classType <= 4)
	{
		return 0;
	}

	int index = classType - 5;
	if (index < 0 || index >= MOD->m_Classes.size())
	{
		return 0;
	}

	return MOD->m_Classes.at(index)->CanEquipItem(item);
}

GETTER_VAR(void*, ASMOnCanEquipItem_jmpback);
GETTER_VAR(void*, ASMOnCanEquipItem_jmpback_2);
GETTER_VAR(void*, ASMOnCanEquipItem_bail);
extern "C" void ASMOnCanEquipItem();

void InitializeOnCanEquipItemHandler()
{
	WriteFarJMP(CWOffset(0x1094E9), (void*)&ASMOnCanEquipItem);
	ASMOnCanEquipItem_jmpback = CWOffset(0x1094F7);
	ASMOnCanEquipItem_jmpback_2 = CWOffset(0x1095CF);
	ASMOnCanEquipItem_bail = CWOffset(0x109677);
}