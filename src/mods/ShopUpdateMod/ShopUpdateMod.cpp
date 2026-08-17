#include "ShopUpdateMod.h"
#include <cstdlib>
#include <cmath>

static void UpdateGemTraderShop(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, long long id);
static void UpdateItemVendorShop(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, long long id);

int ShopUpdateMod::OnShopInteraction(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id)
{
	if (!game || !itemVector || itemVector->empty())
	{
		return 0;
	}

	switch (classType)
	{
	case (int)cube::Enums::ClassType::GemTrader:
		UpdateGemTraderShop(game, itemVector, id);
		return 1;
	case (int)cube::Enums::ClassType::ItemVendor:
		UpdateItemVendorShop(game, itemVector, id);
		return 1;
	default:
		break;
	}
	return 0;
}

static void UpdateGemTraderShop(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, long long id)
{
	if (!game || !game->world || !itemVector || itemVector->empty())
	{
		return;
	}

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
		cube::Item item = cube::SpiritCube::Create((cube::SpiritCube::Type)((int)cube::SpiritCube::Type::FireSpirit + (std::rand() % 4)));

		std::srand(static_cast<unsigned int>(std::rand()));		
		int count = 1 + std::rand() % 3;
		std::srand(seed);
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}

	// Add 3 potions of rarity 1 - 4
	for (int i = 0; i < 4; i++)
	{
		cube::Item item = cube::Item(1, 1);
		item.rarity = i;

		int count = 3;
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}

	// Add 1 artifact
	{
		// Set the random seed to the id of the creature to generate the same items for the same creature every time.
		cube::Item item = cube::Item(23, std::rand() % 7);
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
	if (!game || !game->world || !itemVector || itemVector->empty())
	{
		return;
	}

	cube::Creature* player = game->GetPlayer();
	IntVector2 region(0, 0);
	if (player)
	{
		region = player->entity_data.current_region;
	}

	unsigned int seed = static_cast<unsigned int>(std::abs(id));
	if (seed == 0) seed = 1;
	std::srand(seed);

	const static int SPECIALS[] = {
		1, 2, 3, 18, 12,
	};

	int max = sizeof(SPECIALS) / sizeof(*SPECIALS);
	for (int i = 0; i < max; i++)
	{
		cube::Item item = cube::Item(24, SPECIALS[i]);
		item.rarity = 0;
		item.region = region;

		if (!cube::Helper::CWGetItemsSold(game->world, item, id))
		{
			itemVector->at(0).push_back(cube::ItemStack(1, item));
		}
	}

	{
		std::srand(seed + static_cast<unsigned int>(game->host.world.state.day));
		cube::Item item = cube::SpiritCube::Create((cube::SpiritCube::Type)((int)cube::SpiritCube::Type::FireSpirit + std::rand() % 4));

		std::srand(static_cast<unsigned int>(std::rand()));
		int count = 1 + std::rand() % 3;
		int sold = cube::Helper::CWGetItemsSold(game->world, item, id);

		if (sold < count)
		{
			itemVector->at(0).push_back(cube::ItemStack(count - sold, item));
		}
	}
}