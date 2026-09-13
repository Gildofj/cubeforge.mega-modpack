#pragma once

#include "BaseMod.h"
#include "CharacterClass.h"
#include <vector>

class PlayerUpdatesMod : public BaseMod
{
public:
	std::vector<CharacterClass*> m_Classes;

	PlayerUpdatesMod();
	virtual ~PlayerUpdatesMod() override;

	void Initialize() override;
	void OnGameTick(cube::Game* game) override;
};
