#pragma once
#include "cwsdk.h"
#include "../CubeMod.h"
#include "../cwsdk-extension.h"
#include "../hooks.h"

extern "C" int OnTalkToCreature(cube::Game * game, cube::Creature * creature);

GETTER_VAR(void*, ASMOnTalkToCreature_jmpback);
GETTER_VAR(void*, ASMOnTalkToCreature_jmpback2);
GETTER_VAR(void*, ASMOnTalkToCreature_bail);
extern "C" void ASMOnTalkToCreature();

inline void SetupCreatureInteraction()
{
	WriteFarJMP(CWOffset(0x98D04), &ASMOnTalkToCreature);
	ASMOnTalkToCreature_jmpback = CWOffset(0x98D15);
	ASMOnTalkToCreature_jmpback2 = CWOffset(0x98ED2);
	ASMOnTalkToCreature_bail = CWOffset(0x988C1);
}