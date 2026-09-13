#pragma once

#include "cwsdk.h"
#include <ctime>

namespace cube
{
	class DButton {
	private:
		time_t last_time;
		time_t current_time;
		BYTE oldState;
		BYTE currentState;

	public:
		enum class State
		{
			None = 0,
			Pressed,
			DoubleTap,
			Held
		};

		int diKey;
		DButton(int diKey);
		void Update(BYTE* diKeys);
		State Pressed();
		void SetKey(int dikey);
		const char* GetKeyName();
	};
}
