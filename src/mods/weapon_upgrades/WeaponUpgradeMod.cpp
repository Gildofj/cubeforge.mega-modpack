#include "WeaponUpgradeMod.h"
#include "creature/Creature.h"
#include "memory/MemoryHelper.h"

extern "C" {
	void* ASMWeaponUpgradePreview_jmpback = nullptr;

	void ASMWeaponUpgradePreview();
	void ASMOnWeaponUpgrade();

	void SetWeaponPreview(cube::Item* dest, cube::Item* src)
	{
		if (!dest || !src) return;
		dest->Copy(src);
		dest->UpgradeItem();
	}

	void UpgradeWeapon(cube::AdaptionWidget* widget)
	{
		if (!widget || !widget->game) return;
		widget->game->PrintMessage(L"Upgrading!\n");

		cube::Item* item = widget->item;
		cube::Creature* player = widget->game->GetPlayer();
		if (!item || !player) return;

		int price = item->GetPrice();
		if (price > player->gold)
		{
			return;
		}

		player->gold -= price;
		item->UpgradeItem();
		widget->game->AnnounceReceiptOfItem(item);
	}
}

WeaponUpgradeMod::WeaponUpgradeMod()
	: BaseMod("Weapon Upgrade Mod", "cubeforge-weapon-upgrades", 9, { 1, 0, 0 })
{
}

void WeaponUpgradeMod::Initialize()
{
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x267BD5), (void*)&ASMWeaponUpgradePreview);
	ASMWeaponUpgradePreview_jmpback = (void*)CWOffset(0x267C89);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x267930), (void*)&ASMOnWeaponUpgrade);

	// Patch do layout de botões de upgrade
	unsigned char patch[] = { 0xF3, 0x41, 0x0F, 0x10, 0xF2, 0x90 };
	MemoryHelper::PatchMemory((void*)CWOffset(0x2685D7), patch, sizeof(patch));
	MemoryHelper::PatchMemory((void*)CWOffset(0x268754), patch, sizeof(patch));
}

int WeaponUpgradeMod::OnCreatureTalk(cube::Game* game, cube::Creature* creature)
{
	if (!game || !creature) return 0;

	if (creature->entity_data.classType == static_cast<int>(cube::Enums::ClassType::Smithy))
	{
		if (game->gui.adaption_widget && game->gui.adaption_widget->node)
		{
			game->gui.adaption_widget->item = nullptr;
			plasma::Display* adaptionWidget = game->gui.adaption_widget->node->display;
			adaptionWidget->SetVisibility(adaptionWidget->visibility.current_frame, 1);

			if (game->gui.enchant_widget && game->gui.enchant_widget->node)
			{
				plasma::Display* enchantWidget = game->gui.enchant_widget->node->display;
				enchantWidget->SetVisibility(enchantWidget->visibility.current_frame, 1);

				((void (*)(cube::GUI*, int))CWOffset(0x102DB0))(&game->gui, 0);

				enchantWidget->SetVisibility(enchantWidget->visibility.current_frame, 0);
			}
			return 1;
		}
	}
	return 0;
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new WeaponUpgradeMod();
}
#endif
