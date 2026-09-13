#pragma once

#include "cwsdk.h"
#include "BaseMod.h"
#include <vector>

class MegaModPack : public GenericMod
{
private:
	std::vector<BaseMod*> m_Mods;
	std::vector<BaseMod*> m_ActiveMods;

public:
	MegaModPack();
	virtual ~MegaModPack();

	void Initialize() override;
	void OnGameTick(cube::Game* game) override;
	int OnChat(std::wstring* message) override;
	void OnGetKeyboardState(BYTE* diKeys) override;

	void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override;
	void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) override;
	void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override;
	void OnCreatureHasteCalculated(cube::Creature* creature, float* haste) override;
	void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override;
	void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override;
	void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) override;
	void OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration) override;

	const std::vector<BaseMod*>& GetMods() const { return m_Mods; }
};
