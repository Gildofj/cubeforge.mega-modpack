#include "QuestMod.h"
#include "Quest.h"
#include "inventory/Inventory.h"
#include "creature/Creature.h"
#include "utils/GameHelper.h"
#include "memory/MemoryHelper.h"
#include <algorithm>
#include <climits>

extern "C" {
	void* ASMOnGetItemDescription_jmpback = nullptr;
	void* ASMOnGetItemDescription_jmpback_2 = nullptr;
	void* ASMOnGetItemDescription_bail = nullptr;

	void* ASMOnGetItemName_jmpback = nullptr;
	void* ASMOnGetItemName_bail = nullptr;

	void ASMOnGetItemDescription();
	void ASMOnGetItemName();

	int OnGetItemDescription(cube::BaseWidget* widget, cube::Item* item, FloatVector2* pos)
	{
		if (!widget || !item || !pos) return 0;

		if (item->category == 2)
		{
			std::wstring text;
			cube::Quest* quest = (cube::Quest*)item;
			cube::Game* game = cube::GetGame();
			if (!game) return 0;

			text = *quest->GetQuestName(&game->speech, &text);

			FloatRGBA color(1.f, .25f, 0.25f, 1.f);
			widget->SetDrawColor(&color);
			widget->DrawBaseWidgetText(pos, &text, pos->x, pos->y);

			color = { 1.f, 1.f, 1.f, 1.f };
			text = *quest->GetQuestDescription(&game->speech, &text);
			widget->SetDrawColor(&color);
			widget->DrawBaseWidgetText(pos, &text, pos->x, pos->y);
			
			return 1;
		}
		return 0;
	}

	int OnGetItemName(cube::Speech* speech, cube::Item* item, cube::Item* copy)
	{
		(void)speech;
		const static int ID_OFFSET = 15;
		if (item && copy && item->category == 2)
		{
			*copy = cube::Item(1, ID_OFFSET + static_cast<int>(((cube::Quest*)item)->GetType()));
			return 1;
		}
		return 0;
	}
}

QuestMod::QuestMod()
	: BaseMod("Quest Mod", "cubeforge-quest-system", 10, { 1, 2, 0 })
{
}

void QuestMod::OnGameTick(cube::Game* game)
{
	if (!game) return;
	const static int ID_OFFSET = 15;
	static bool init = false;
	if (!init)
	{
		init = true;
		game->speech.item_identifier_map.insert_or_assign(std::make_pair<uint32_t, uint32_t>(1, ID_OFFSET), L"QuestKill");
		game->speech.item_identifier_map.insert_or_assign(std::make_pair<uint32_t, uint32_t>(1, ID_OFFSET + 1), L"QuestGather");
		game->speech.item_identifier_map.insert_or_assign(std::make_pair<uint32_t, uint32_t>(1, ID_OFFSET + 2), L"QuestDeliver");
		game->speech.item_identifier_map.insert_or_assign(std::make_pair<uint32_t, uint32_t>(1, ID_OFFSET + 3), L"QuestTalk");
	}
}

void QuestMod::Initialize()
{
	MemoryHelper::FindAndReplaceString(L"Formula: ", L"[Quest]: ");

	MemoryHelper::WriteFarJMP((void*)CWOffset(0x1628E4), (void*)&ASMOnGetItemName);
	ASMOnGetItemName_jmpback = (void*)CWOffset(0x1628F5);
	ASMOnGetItemName_bail = (void*)CWOffset(0x1629A4);

	MemoryHelper::WriteFarJMP((void*)CWOffset(0xBA33B), (void*)&ASMOnGetItemDescription);
	ASMOnGetItemDescription_jmpback = (void*)CWOffset(0xBA351);
	ASMOnGetItemDescription_jmpback_2 = (void*)CWOffset(0xBA478);
	ASMOnGetItemDescription_bail = (void*)CWOffset(0xBA363);
}

void QuestMod::OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker)
{
	if (!creature || !attacker) return;

	cube::Game* game = cube::GetGame();
	if (!game || !game->world) return;

	cube::Creature* foundPlayer = nullptr;
	for (cube::Creature* c : game->world->creatures)
	{
		if (!c) continue;
		if (c->entity_data.hostility_type == static_cast<int>(cube::Enums::EntityBehaviour::Player))
		{
			if (c->id == attacker->id || c->pet_id == attacker->id)
			{
				foundPlayer = c;
				break;
			}
		}
	}

	if (foundPlayer == nullptr || foundPlayer->inventory_tabs.size() <= static_cast<size_t>(cube::Inventory::IngredientsTab))
	{
		return;
	}

	auto ingredients = &foundPlayer->inventory_tabs.at(cube::Inventory::IngredientsTab);
	for (int i = static_cast<int>(ingredients->size()) - 1; i >= 0; i--)
	{
		cube::ItemStack* stack = &ingredients->at(i);
		if (stack->item.category == 2)
		{
			cube::Quest* quest = (cube::Quest*)&stack->item;
			if (quest->GetType() == cube::Quest::QuestType::Killing)
			{
				if (quest->GetSubType() == static_cast<int>(creature->entity_data.race))
				{
					quest->IncreaseProgress();
					if (quest->IsCompleted())
					{
						quest->Complete();
						ingredients->erase(ingredients->begin() + i);
					}
				}
			}
		}
	}
}

int QuestMod::OnCreatureTalk(cube::Game* game, cube::Creature* creature)
{
	if (!game || !creature) return 0;

	if (std::find(m_TalkedIds.begin(), m_TalkedIds.end(), creature->id) != m_TalkedIds.end())
	{
		return 0;
	}
	m_TalkedIds.push_front(creature->id);

	if (creature->id % 5 != 0)
	{
		return 0;
	}

	long long mod = std::abs(creature->id / 5);
	while (mod > INT_MAX)
	{
		mod = mod >> 2;
	}

	cube::Item item(2, 0);
	item.modifier = static_cast<int>((mod / 4) * 4);
	item.rarity = static_cast<char>(mod % 5);

	cube::Creature* player = game->GetPlayer();
	if (player)
	{
		cube::Helper::DropItem(player, item, 1);
	}

	return 0;
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new QuestMod();
}
#endif
