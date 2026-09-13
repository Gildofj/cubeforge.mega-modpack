#include "LoreInteractionMod.h"
#include "utils/GameHelper.h"
#include "memory/MemoryHelper.h"
#include <cwchar>
#include <algorithm>

extern "C" {
	void* ASMIncreaseLore_jmpback = nullptr;
	void ASMIncreaseLore();
}

static LoreInteractionMod* s_LoreInstance = nullptr;

extern "C" void OnPrintRGBA(std::wstring* message)
{
	if (!message || !s_LoreInstance) return;
	const wchar_t* msg = message->c_str();
	int index = 0;

	if (swscanf_s(msg, L" lore increased to %d", &index) == 1)
	{
		cube::Game* game = cube::GetGame();
		if (game)
		{
			s_LoreInstance->OnLoreIncrease(game, index);
		}
	}
}

static void AnnounceFoundLoreItem(cube::Game* game, int type)
{
	const static wchar_t* places[] = {
		L"hidden behind the description",
		L"burried in the ground",
		L"covered in mud",
		L"next to the object",
		L"by accident",
	};
	const static int places_size = sizeof(places) / sizeof(*places);

	const static wchar_t* verbs[] = {
		L"found",
		L"stumbled upon",
		L"came accross",
		L"detected",
		L"uncovered",
		L"noticed",
	};
	const static int verbs_size = sizeof(verbs) / sizeof(*verbs);

	const static wchar_t* item_types[] = {
		L"some gear",
		L"a weapon",
		L"an artifact",
	};
	const static wchar_t* endings[] = {
		L"!",
		L"."
	};

	wchar_t buffer[250];
	swprintf_s(buffer, 250,
		L"You %s %s %s%s\n",
		verbs[cube::Helper::RandomInt() % verbs_size],
		item_types[type],
		places[cube::Helper::RandomInt() % places_size],
		endings[cube::Helper::RandomInt() % 2]
	);

	game->PrintMessage(buffer, 255, 165, 0);
}

LoreInteractionMod::LoreInteractionMod()
	: BaseMod("Lore Interaction Mod", "cubeforge-lore-interactions", 2, { 1, 1, 0 })
{
	s_LoreInstance = this;
}

LoreInteractionMod::~LoreInteractionMod()
{
	if (s_LoreInstance == this)
	{
		s_LoreInstance = nullptr;
	}
}

void LoreInteractionMod::Initialize()
{
	ASMIncreaseLore_jmpback = (void*)CWOffset(0x11B34D);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x11B348), (void*)&ASMIncreaseLore);
}

void LoreInteractionMod::OnLoreIncrease(cube::Game* game, int value)
{
	if (!game) return;
	cube::Creature* player = game->GetPlayer();
	if (!player) return;

	int chance = cube::Helper::RandomInt() % 100;
	int compare = 0;
	if (value < 100)
	{
		compare = (std::min)(value, 50);
		if (chance <= compare)
		{
			cube::Item item = cube::Helper::GenerateItem(cube::Helper::ItemGenerationType::Gear, player->entity_data.current_region);
			item.rarity = 0;
			cube::Helper::DropItem(player, item);
			AnnounceFoundLoreItem(game, 0);
		}
	}
	else if (value < 150)
	{
		compare = (value - 100);
		if (chance <= compare)
		{
			cube::Item item = cube::Helper::GenerateItem(cube::Helper::ItemGenerationType::Gear, player->entity_data.current_region);
			item.rarity = 1 + cube::Helper::RandomInt() % 2;
			item.category = 3;
			item.id = cube::Helper::RandomInt() % 18;
			cube::Helper::DropItem(player, item);
			AnnounceFoundLoreItem(game, 1);
		}
	}
	else
	{
		compare = (std::min)((value - 150) / 2, 40);
		if (chance <= compare)
		{
			cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Artifact);
			AnnounceFoundLoreItem(game, 2);
		}
	}
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new LoreInteractionMod();
}
#endif
