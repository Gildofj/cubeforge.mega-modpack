#pragma once

#include "Ability.h"

namespace cube
{
	class HealAbility : public Ability
	{
	public:
		HealAbility();
		~HealAbility() override;

		void Execute(cube::Creature* player) override;
	};
}
