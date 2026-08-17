#pragma once
#include "../hooks.h"
#include <string>

extern "C" void OnCreatureDeath(cube::Creature * creature, cube::Creature * attacker);

GETTER_VAR(void*, ASMOnCreatureDeath_jmpback);
extern "C" void ASMOnCreatureDeath();

inline void SetupOnCreatureDeathHandler()
{
	WriteFarJMP(CWOffset(0x29E494), ASMOnCreatureDeath);
	ASMOnCreatureDeath_jmpback = CWOffset(0x29E4A5);
}