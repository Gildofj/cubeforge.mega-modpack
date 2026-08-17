#pragma once
#include "cwsdk.h"
#include "../CubeMod.h"

extern "C" void OnItemPriceHandler(cube::Item* item, int* price);
extern "C" float callPowf(float a1, float a2);

GETTER_VAR(void*, ASMItemPrice_jmpback);
extern "C" void ASMItemPriceHandler();

inline void SetupItemPriceHandler() {
	WriteFarJMP(CWOffset(0x109E0E), (void*)&ASMItemPriceHandler);
	ASMItemPrice_jmpback = CWOffset(0x109E2E);
}