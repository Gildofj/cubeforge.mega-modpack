#include "AddGoldEvent.h"

cube::AddGoldEvent::AddGoldEvent()
	: Event()
{
	m_PrintTimer = Timer(1.0, m_CurrentTime);
	eventType = cube::EventType::AddGold;

	cube::Game* game = cube::GetGame();
	if (game)
	{
		game->PrintMessage(L"[Event Started] ", 100, 100, 255);
		game->PrintMessage(L"AddGold Event\n");
	}
}

cube::AddGoldEvent::~AddGoldEvent()
{
	cube::Game* game = cube::GetGame();
	if (game)
	{
		game->PrintMessage(L"[Event Ended] ", 100, 100, 255);
		game->PrintMessage(L"AddGold Event\n");
	}
}

void cube::AddGoldEvent::Update()
{
	Event::Update();

	if (m_PrintTimer.IsTriggered(m_CurrentTime))
	{
		cube::Game* game = cube::GetGame();
		if (game && game->GetPlayer())
		{
			game->GetPlayer()->gold++;
		}
	}
}
