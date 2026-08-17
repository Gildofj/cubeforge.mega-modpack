#pragma once
#include "cwsdk.h"
#include "../CubeMod.h"
#include "../cwsdk-extension.h"
#include "../hooks.h"

inline int ChestInteractionHandler(cube::Game* game, cube::Creature* creature, int type) {
	if (!game || !creature)
	{
		return 0;
	}

	for (CubeMod* mod : g_Mods) {
		if (mod && mod->OnChestInteraction(game, creature, type)) {
			return 1;
		}
	}
	return 0;
}

extern "C" void OnChestInteraction(cube::Game* game, cube::Creature* creature);

GETTER_VAR(void*, ASMIsChestOpen_jmpback);
GETTER_VAR(void*, ASMIsChestOpen_bail);
extern "C" void ASMIsChestOpen();

GETTER_VAR(void*, ASMChestInteractionHandler_jmpback);
extern "C" void ASMChestInteractionHandler();

inline void SetupChestInteractionHandler() {
	WriteFarJMP(g_Base + 0x99288, (void*)&ASMChestInteractionHandler);
	ASMChestInteractionHandler_jmpback = (void*)(g_Base + 0x988C1);

	// Todo: Create an OnCreateInteraction(cube::Creature*) function.
	WriteFarJMP(g_Base + 0x991A3, (void*)&ASMIsChestOpen);
	ASMIsChestOpen_jmpback = (void*)(g_Base + 0x991C6);
	ASMIsChestOpen_bail = (void*)(g_Base + 0x99298);
}