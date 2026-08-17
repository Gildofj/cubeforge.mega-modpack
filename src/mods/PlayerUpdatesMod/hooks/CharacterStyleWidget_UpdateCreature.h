#pragma once
#include "cwsdk.h"

extern "C" void CharacterStyleWidgetUpdateCreature(cube::CharacterStyleWidget * this) {

}

extern "C" void ASMCharacterStyleWidgetUpdateCreature();

void InitializeCharacterStyleWidgetUpdateCreatureHandler() {
	WriteFarJMP(CWOffset(0x272D90), (void*)&ASMCharacterStyleWidgetUpdateCreature);
}