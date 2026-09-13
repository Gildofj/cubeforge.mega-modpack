#pragma once

#include "BaseMod.h"

class ShopUpdateMod : public BaseMod
{
public:
	ShopUpdateMod();
	virtual ~ShopUpdateMod() override;

	void Initialize() override;
	int OnShopInteraction(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id) override;
};
