#pragma once

#include "BaseMod.h"
#include <list>

class QuestMod : public BaseMod
{
private:
	std::list<long long> m_TalkedIds;
public:
	QuestMod();
	virtual ~QuestMod() override = default;

	void OnGameTick(cube::Game* game) override;
	void Initialize() override;
	void OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker) override;
	int OnCreatureTalk(cube::Game* game, cube::Creature* creature) override;
};
