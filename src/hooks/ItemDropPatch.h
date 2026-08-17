#pragma once
#include "cwsdk.h"

inline IntVector2* GetPlayerRegion(void)
{
	return &cube::GetGame()->GetPlayer()->entity_data.current_region;
}

extern "C" IntVector2* GetDropRegion(void);

GETTER_VAR(void*, ASMPatchItemDrop_jmpback);
extern "C" void ASMPatchItemDrop();

inline void ItemDropPatchInitialize() {
	char* base = (char*)CWBase();
	WriteFarJMP(base + 0x2A5E2E, (void*)&ASMPatchItemDrop);
	WriteByte(CWOffset(0x2A5E3C), 0x90);
	ASMPatchItemDrop_jmpback = (void*)(base + 0x2A5E3D);
}