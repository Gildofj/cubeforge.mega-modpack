#pragma once
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include "cwsdk.h"

GETTER_VAR(void*, ASMTreasureFixHandler_ret_1);
GETTER_VAR(void*, ASMTreasureFixHandler_ret_2);
GETTER_VAR(void*, ASMTreasureFixHandler_jmpback);

extern "C" int GetTreasureDropClass(cube::Creature* player) {
	if (player->entity_data.classType <= 4)
	{
		return player->entity_data.classType;
	}
	
	int index = player->entity_data.classType - 5;
	if (index < 0 || index >= MOD->m_Classes.size())
	{
		srand(time(0));  // Initialize random number generator.
		return (rand() % 4) + 1;
	}

	int res = MOD->m_Classes.at(index)->GetTreasureClassType(player);
	if (res == 0)
	{
		srand(time(0));  // Initialize random number generator.
		return (rand() % 4) + 1;
	}
	return res;
}

extern "C" void ASMTreasureFixHandler();


void InitializeTreasureFixHandler() {
	WriteFarJMP(CWOffset(0x10A7FE), (void*)&ASMTreasureFixHandler);
	ASMTreasureFixHandler_jmpback = CWOffset(0x10A810);
	ASMTreasureFixHandler_ret_1 = CWOffset(0x10AB70);
	ASMTreasureFixHandler_ret_2 = CWOffset(0x10AA60);
}