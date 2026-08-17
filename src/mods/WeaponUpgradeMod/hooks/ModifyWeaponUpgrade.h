/* Hook for modifying how weapons get upgraded.
*/
#include "cwsdk.h"

extern "C" void SetWeaponPreview(cube::Item * dest, cube::Item * src) {
	dest->Copy(src);
	dest->UpgradeItem();
	return;
}

extern "C" void UpgradeWeapon(cube::AdaptionWidget * widget) {
	widget->game->PrintMessage(L"Upgrading! \n");

	cube::Item* item = widget->item;
	cube::Creature* player = widget->game->GetPlayer();
	int price = item->GetPrice();
	if (price > player->gold)
	{
		return;
	}

	player->gold -= price;
	item->UpgradeItem();
	widget->game->AnnounceReceiptOfItem(item);
	return;
}

GETTER_VAR(void*, ASMWeaponUpgradePreview_jmpback);
extern "C" void ASMWeaponUpgradePreview();
extern "C" void ASMOnWeaponUpgrade();

void SetupWeaponUpgradeModification()
{
	WriteFarJMP(CWOffset(0x267BD5), &ASMWeaponUpgradePreview);
	ASMWeaponUpgradePreview_jmpback = CWOffset(0x267C89);

	WriteFarJMP(CWOffset(0x267930), &ASMOnWeaponUpgrade);
}