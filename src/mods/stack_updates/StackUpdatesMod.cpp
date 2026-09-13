#include "StackUpdatesMod.h"
#include "memory/MemoryHelper.h"

StackUpdatesMod::StackUpdatesMod()
	: BaseMod("Stack Updates Mod", "cubeforge-stack-updates", 12, { 1, 0, 0 })
{
}

void StackUpdatesMod::Initialize()
{
	auto offset_p1 = 0x50755;	// jne -> jmp: Garante que todos os tamanhos de stack sejam iguais
	auto offset_p2 = 0x50789;	// 50 -> 100: Aumenta limite de stack

	MemoryHelper::WriteByte((void*)CWOffset(offset_p1), 0xEB);
	MemoryHelper::WriteByte((void*)CWOffset(offset_p2 + 3), 0x64);
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new StackUpdatesMod();
}
#endif
