#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

#include "cwsdk.h"
#include "memory/MemoryHelper.h"

struct ModVersion
{
	int major;
	int minor;
	int sub;

	ModVersion() : major(1), minor(0), sub(0) {}
	ModVersion(int maj, int min, int s) : major(maj), minor(min), sub(s) {}

	std::string ToString() const
	{
		return "v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(sub);
	}
};

class BaseMod : public GenericMod
{
public:
	std::string m_Name;
	const char* m_FileName;
	int m_ID;
	ModVersion m_Version;
	bool m_Enabled;

	BaseMod(const std::string& name = "BaseMod", const char* fileName = "BaseMod", int id = 0, ModVersion ver = { 1, 0, 0 })
		: m_Name(name), m_FileName(fileName), m_ID(id), m_Version(ver), m_Enabled(true)
	{
	}

	virtual ~BaseMod() = default;

	// Ciclo de Vida do Mod
	virtual void Initialize() override {}
	virtual void OnGameTick(cube::Game* game) override {}
	virtual int OnChat(std::wstring* message) override { return 0; }
	virtual void OnGetKeyboardState(BYTE* diKeys) override {}

	// Eventos de Jogo Customizados
	virtual int OnChestInteraction(cube::Game* game, cube::Creature* creature, int type) { return 0; }
	virtual void OnLoreIncrease(cube::Game* game, int value) {}
	virtual int OnCreatureTalk(cube::Game* game, cube::Creature* creature) { return 0; }
	virtual void OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker) {}
	virtual int OnShopInteraction(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id) { return 0; }

	// Callbacks de Atributos de Criaturas (CWSDK)
	virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) override {}
	virtual void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) override {}
	virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) override {}
	virtual void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) override {}
	virtual void OnCreatureHasteCalculated(cube::Creature* creature, float* haste) override {}
	virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) override {}
	virtual void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) override {}
	virtual void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) override {}
	virtual void OnCreatureManaGenerationCalculated(cube::Creature* creature, float* manaGeneration) override {}

	// Persistência em Disco
	void Save(const void* data, size_t size);
	void Load(void* data, size_t size);
};

// Aliases para retrocompatibilidade
using CubeMod = BaseMod;
