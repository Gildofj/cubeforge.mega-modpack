#pragma once

#include "cwsdk.h"
#include "Event.h"
#include "../inventory/Timer.h"

namespace cube
{
	class AddGoldEvent : public Event
	{
	private:
		Timer m_PrintTimer;
	public:
		AddGoldEvent();
		~AddGoldEvent() override;

		void Update() override;
	};
}
