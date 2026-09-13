#include "ShopUpdateMod.h"
#include "utils/GameHelper.h"
#include "creature/Creature.h"
#include "inventory/Item.h"
#include "memory/MemoryHelper.h"
#include <cstdlib>
#include <cmath>

extern "C" {
	void* ASMShopInteraction_jmpback = nullptr;
	void* ASMItemPrice_jmpback = nullptr;

	void ASMShopInteractionHandler();
	void ASMItemPriceHandler();

	void ShopInteractionSub1(void* p1, void* p2)
	{
		((void (*)(void*, void*))CWOffset(0x64F30))(p1, p2);
	}

	void ShopInteractionSub2(void* p1)
	{
		((void (*)(void*))CWOffset(0x280A40))(p1);
	}

	float callPowf(float a1, float a2)
	{
		return std::pow(a1, a2);
	}
}

static ShopUpdateMod* s_ShopInstance = nullptr;

extern "C" void OnShopInteractionHandler(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id)
{
	if (!game || !itemVector || !s_ShopInstance) return;

	std::vector<std::vector<cube::ItemStack>> temp;
	temp.push_back(std::vector<cube::ItemStack>());

	bool clear = false;
	if (s_ShopInstance->OnShopInteraction(game, &temp, classType, id))
	{
		clear = true;
	}

	if (clear && !itemVector->empty())
	{
		itemVector->at(0).clear();
	}

	if (!temp.empty())
	{
		for (const auto& i : temp.at(0))
		{
			itemVector->at(0).push_back(i);
		}
	}
}

extern "C" void OnItemPriceHandler(cube::Item* item, int* price)
{
	if (!item || !price) return;

	switch (item->category)
	{
	case 1:
		if (item->id == 1)
		{
			*price = 5 * (item->rarity + 1);
		}
		break;
	case 11:
		if (item->id == 14)
		{
			*price = 25;
		}
		break;
	case 23:
		*price = 1000;
		break;
	case 24:
		if (item->id > 13 && item->id < 18)
		{
			*price = 25 * (item->id - 13);
		}
		else if (item->id == 12)
		{
			*price = 25;
		}
		else
		{
			*price = 75;
		}
		break;
	default:
		break;
	}
}

static void UpdateGemTraderShop(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, long long id)
{
	if (!game || !game->world || !itemVector || itemVector->empty()) return;

	unsigned int seed = static_cast<unsigned int>(std::abs(id));
	if (seed == 0) seed = 1;

	std::srand(seed);
	unsigned int chanceSeed = 1 + (std::rand() % seed);
	std::srand(chanceSeed);
	unsigned int chance = std::rand();
	std::srand(seed);

	if (chance % 2 == 0)
	{
		std::srand(seed);
		cube::Item item = cube::SpiritCube::Create(static_cast<cube::SpiritCube::Type>(static_cast<int>(cube::SpiritCube::Type::FireSpirit) + (std::rand() % 4)));

		std::srand(static_cast<unsigned int>(std::rand()));		
		int count = 1 + std::rand() % 3;
		std::srand(seed);
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		cube::Item item(1, 1);
		item.rarity = static_cast<char>(i);

		int count = 3;
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}

	{
		cube::Item item(23, std::rand() % 7);
		item.region = IntVector2(std::rand(), std::rand());
		item.modifier = std::rand();
		item.rarity = 10;
		item.formula_category = std::rand();

		if (!cube::Helper::CWGetItemsSold(game->world, item, id))
		{
			itemVector->at(0).push_back(cube::ItemStack(1, item));
		}
	}
}

static void UpdateItemVendorShop(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, long long id)
{
	if (!game || !game->world || !itemVector || itemVector->empty()) return;

	cube::Creature* player = game->GetPlayer();
	IntVector2 region(0, 0);
	if (player)
	{
		region = player->entity_data.current_region;
	}

	unsigned int seed = static_cast<unsigned int>(std::abs(id));
	if (seed == 0) seed = 1;
	std::srand(seed);

	const static int SPECIALS[] = { 1, 2, 3, 18, 12 };
	int max = sizeof(SPECIALS) / sizeof(*SPECIALS);
	for (int i = 0; i < max; i++)
	{
		cube::Item item(24, SPECIALS[i]);
		item.rarity = 0;
		item.region = region;

		if (!cube::Helper::CWGetItemsSold(game->world, item, id))
		{
			itemVector->at(0).push_back(cube::ItemStack(1, item));
		}
	}

	{
		std::srand(seed + static_cast<unsigned int>(game->host.world.state.day));
		cube::Item item = cube::SpiritCube::Create(static_cast<cube::SpiritCube::Type>(static_cast<int>(cube::SpiritCube::Type::FireSpirit) + std::rand() % 4));

		std::srand(static_cast<unsigned int>(std::rand()));
		int count = 1 + std::rand() % 3;
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}
}

ShopUpdateMod::ShopUpdateMod()
	: BaseMod("Shop Update Mod", "cubeforge-shop-updates", 5, { 1, 2, 0 })
{
	s_ShopInstance = this;
}

ShopUpdateMod::~ShopUpdateMod()
{
	if (s_ShopInstance == this)
	{
		s_ShopInstance = nullptr;
	}
}

void ShopUpdateMod::Initialize()
{
	ASMShopInteraction_jmpback = (void*)CWOffset(0x64FD2);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x64F8C), (void*)&ASMShopInteractionHandler);

	ASMItemPrice_jmpback = (void*)CWOffset(0x269E97);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x269E73), (void*)&ASMItemPriceHandler);
}

int ShopUpdateMod::OnShopInteraction(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id)
{
	if (!game || !itemVector || itemVector->empty()) return 0;

	switch (classType)
	{
	case static_cast<int>(cube::Enums::ClassType::GemTrader):
		UpdateGemTraderShop(game, itemVector, id);
		return 1;
	case static_cast<int>(cube::Enums::ClassType::ItemVendor):
		UpdateItemVendorShop(game, itemVector, id);
		return 1;
	default:
		break;
	}
	return 0;
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new ShopUpdateMod();
}
#endif
