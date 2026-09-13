#include "RegionLockMod.h"
#include "memory/MemoryHelper.h"

extern "C" {
	void* ASMPatchItemDrop_jmpback = nullptr;

	void ASMEffectiveItemRarity();
	void ASMPatchItemDrop();

	IntVector2* GetDropRegion(void)
	{
		cube::Game* game = cube::GetGame();
		if (!game) return nullptr;
		cube::Creature* player = game->GetPlayer();
		if (!player) return nullptr;
		return &player->entity_data.current_region;
	}

	int OnGetEffectiveItemRarity(cube::Item* item, int distance)
	{
		if (!item) return 0;
		if (item->category == 2)
		{
			return item->rarity;
		}

		int plus = item->IsPlusItem() ? 1 : 0;
		long long calc = static_cast<long long>(item->rarity) - static_cast<long long>((2 - plus) * distance);
		if (calc < 0)
		{
			return -1;
		}
		return static_cast<int>(calc);
	}
}

RegionLockMod::RegionLockMod()
	: BaseMod("Region Lock Update Mod", "cubeforge-region-lock", 8, { 1, 0, 0 })
{
}

void RegionLockMod::Initialize()
{
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x10976D), (void*)&ASMEffectiveItemRarity);
	MemoryHelper::WriteByte((void*)CWOffset(0x10974B), 0x70);
	MemoryHelper::WriteByte((void*)CWOffset(0x109733), 0x70);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2A5E2E), (void*)&ASMPatchItemDrop);
	MemoryHelper::WriteByte((void*)CWOffset(0x2A5E3C), 0x90);
	ASMPatchItemDrop_jmpback = (void*)CWOffset(0x2A5E3D);
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new RegionLockMod();
}
#endif
