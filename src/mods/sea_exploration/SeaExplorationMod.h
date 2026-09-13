#pragma once

#include "BaseMod.h"
#include "DivingEvent.h"

struct SeaExplorationData
{
	bool autoGoldUsage;
};

class SeaExplorationMod : public BaseMod
{
private:
	SeaExplorationData m_Data;
	cube::DivingEvent* m_DivingEvent;

public:
	SeaExplorationMod();
	virtual ~SeaExplorationMod() override;

	void Initialize() override;
	void OnGameTick(cube::Game* game) override;
	int OnChat(std::wstring* message) override;
	int OnChestInteraction(cube::Game* game, cube::Creature* creature, int type) override;
};
