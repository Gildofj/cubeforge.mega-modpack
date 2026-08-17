#pragma once
#include "cwsdk.h"
#include "../CubeMod.h"

extern "C" void OnShopInteractionHandler(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id);
extern "C" void ShopInteractionSub1(void* p1, void* p2);
extern "C" void ShopInteractionSub2(void* p1);

GETTER_VAR(void*, ASMShopInteraction_jmpback);
extern "C" void ASMShopInteractionHandler();

inline void SetupShopInteractionHandler() {
	WriteFarJMP(CWOffset(0xB7709), (void*)&ASMShopInteractionHandler);
	ASMShopInteraction_jmpback = CWOffset(0xB7722);
}