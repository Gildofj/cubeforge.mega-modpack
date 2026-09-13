#include "Ability.h"
#include "cwsdk.h"

cube::Ability::Ability()
{
}

cube::Ability::~Ability()
{
}

void cube::Ability::Execute(cube::Creature* player)
{
	(void)player;
}

void cube::Ability::CWAbility(cube::Creature* player, int id)
{
	if (player)
	{
		player->entity_data.current_ability = id;
		player->entity_data.time_since_ability = 0;
	}
}

void cube::Ability::CWBuff(cube::Creature* player, int id, float duration)
{
	(void)player;
	(void)id;
	(void)duration;
}
