#include "DButton.h"
#include <sys/timeb.h>

cube::DButton::DButton(int diKey) {
	oldState = 0;
	currentState = 0;
	last_time = 0;
	current_time = 0;
	SetKey(diKey);
}

void cube::DButton::Update(BYTE* diKeys) {
	oldState = currentState;
	currentState = diKeys[diKey];

	if (((oldState & 0x80) == 0) && ((currentState & 0x80) != 0))
	{
		struct _timeb timebuffer;
		_ftime64_s(&timebuffer);
		last_time = current_time;
		current_time = (timebuffer.time * 1000) + (timebuffer.millitm);
	}	
}

cube::DButton::State cube::DButton::Pressed() {
	const static int DOUBLE_PRESS = 350;

	bool ret = ((oldState & 0x80) == 0) && ((currentState & 0x80) != 0);
	if (ret)
	{
		if (difftime(current_time, last_time) <= DOUBLE_PRESS)
		{
			return State::DoubleTap;
		}
		return State::Pressed;
	}

	if ((oldState & 0x80) != 0)
	{
		return State::Held;
	}
	return State::None;
}

void cube::DButton::SetKey(int dikey) {
	this->diKey = dikey;
}

const char* cube::DButton::GetKeyName() {
	static const char* keys[] = {
		"KEY 0x00", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACK", "TAB",
		"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "RETURN", "LCTRL",
		"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LSHIFT", "\\",
		"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RSHIFT", "MULTIPLY", "LMENU", "SPACE", "CAPITAL",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUMLOCK", "SCROLL",
		"NUM7", "NUM8", "NUM9", "SUBTRACT", "NUM4", "NUM5", "NUM6", "ADD", "NUM1", "NUM2", "NUM3", "NUM0", "DECIMAL"
	};
	if (diKey >= 0 && diKey < sizeof(keys) / sizeof(*keys))
	{
		return keys[diKey];
	}
	return "UNKNOWN";
}
