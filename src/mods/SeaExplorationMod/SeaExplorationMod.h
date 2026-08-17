#pragma once
#include "../../CubeMod.h"

#include "event/DivingEvent.h"

struct SeaExplorationData
{
	bool autoGoldUsage;
};

class SeaExplorationMod : public CubeMod
{
private:
	SeaExplorationData m_Data;
	cube::DivingEvent* m_DivingEvent;

public:
	SeaExplorationMod() {
		m_Name = "Sea Exploration Mod";
		m_FileName = "SeaExplorationMod";
		m_ID = 1;
		m_Version = { 1, 3, 0 };
		m_DivingEvent = nullptr;

		m_Data = { true };
		Load(&m_Data, sizeof(m_Data));
	}

	virtual ~SeaExplorationMod() {
		if (m_DivingEvent) {
			delete m_DivingEvent;
			m_DivingEvent = nullptr;
		}
	}

	int OnChestInteraction(cube::Game* game, cube::Creature* creature, int type) override;
	void OnGameTick(cube::Game* game) override;
	int OnChat(std::wstring* message) override;
	void Initialize() override;
};