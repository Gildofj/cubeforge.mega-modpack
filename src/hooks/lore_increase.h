#pragma once
#include "cwsdk.h"
#include "../cwsdk-extension/creature/Creature.h"

inline std::vector<hook::HookEventData>* g_hookEvents = nullptr;
const static int LORE_LEVELUP_THRESHHOLD = 100;

extern "C" void OnPrintRGBA(std::wstring* message);

GETTER_VAR(void*, ASMIncreaseLore_jmpback);
extern "C" void ASMIncreaseLore();

inline void SetupIncreaseLoreHandler() {
	char* base = (char*)CWBase();
	WriteFarJMP(base + 0x26B6C4, (void*)&ASMIncreaseLore);
	ASMIncreaseLore_jmpback = (void*)(base + 0x26B6D9);
}

inline void IncreaseLoreInitialize(std::vector<hook::HookEventData>* events)
{
	g_hookEvents = events;
	SetupIncreaseLoreHandler();
}