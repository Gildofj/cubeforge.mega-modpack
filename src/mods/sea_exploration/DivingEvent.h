#pragma once

#include "cwsdk.h"
#include <vector>

#include "abilities/Event.h"
#include "inventory/Timer.h"
#include "creature/Creature.h"
#include "creature/CreatureFactory.h"

// Constantes de Spawning
const static double FISH_SPAWN_INTERVAL = 1.0;
const static auto FISH_SPAWN_AMOUNT = 4;
const static auto MAX_FISH_COUNT = 100;

const static double TREASURE_SPAWN_INTERVAL = 60.0;
const static auto MAX_TREASURE_COUNT = 5;

const static double BOSS_SPAWN_INTERVAL = 10.0;
const static auto MAX_BOSS_COUNT = 1;

const static auto BOUNDS_CHECK_DIST_MULTIPLIER = 2;
const static long long CREATURE_SPAWN_RANGE = 5000000;
const static double BOUNDS_CHECK_INTERVAL = 5.0;

struct IdCreaturePair
{
	__int64 id;
	cube::Creature* creature;
};

struct SpawnMoment
{
	LongVector3 position;
	std::vector<IdCreaturePair> creature_ids;
};

namespace cube
{
	class DivingEvent : public Event
	{
	private:
		cube::Timer* m_ItemEffectTimer;

		cube::Timer m_SpawnTimer;
		cube::Timer m_TreasureTimer;
		cube::Timer m_BossTimer;
		cube::Timer m_BoundsTimer;

		std::vector<SpawnMoment> m_SpawnedCreatures;
		std::vector<IdCreaturePair> m_SpawnedTreasures;
		std::vector<IdCreaturePair> m_SpawnedBosses;

		bool* autoGoldUsage;

	public:
		DivingEvent(bool* autoGoldUsage);
		~DivingEvent() override;

		void Update() override;
	private:
		void UpdateIds();
		void SetDiving(bool diving);

		void HandleItemEffectTimer();
		void ConsumeItem();

		void HandleFishTimer();
		void SpawnFishes(const LongVector3& position);

		void HandleTreasureTimer();
		void SpawnTreasures(const LongVector3& position);
		int GenerateChestType();

		void HandleBossTimer();
		void SpawnBoss(const LongVector3& position);

		void HandleBoundsCheckTimer();
		void BoundCheckFishes(const LongVector3& position);
		void BoundCheckTreasures(const LongVector3& position);
		void BoundCheckBosses(const LongVector3& position);
	};
}
