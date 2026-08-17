/*
* Hook for letting smithy interaction open the inventory and adaption widget.
*/
#include "cwsdk.h"
#include "../../../cwsdk-extension.h"

extern "C" int OnTalkToCreature(cube::Game* game, cube::Creature* creature) 
{
	if (creature->entity_data.classType == (int)cube::Enums::ClassType::Smithy)
	{
		game->gui.adaption_widget->item = nullptr;
		plasma::Display* adaptionWidget = game->gui.adaption_widget->node->display;
		adaptionWidget->SetVisibility(adaptionWidget->visibility.current_frame, 1);

		// Enable enchant widget to avoid chrash
		plasma::Display* characterWidget = game->gui.enchant_widget->node->display;
		characterWidget->SetVisibility(characterWidget->visibility.current_frame, 1);
		
		// Open inventory widget code
		((void (*)(cube::GUI*, int))CWOffset(0x102DB0))(&game->gui, 0);

		// Hide enchant widget again
		characterWidget->SetVisibility(characterWidget->visibility.current_frame, 0);
		return 1;
	}
	return 0;
}

GETTER_VAR(void*, ASMOnTalkToCreature_jmpback);
GETTER_VAR(void*, ASMOnTalkToCreature_jmpback2);
GETTER_VAR(void*, ASMOnTalkToCreature_bail);
extern "C" void ASMOnTalkToCreature();


void SetupCreatureInteraction()
{
	WriteFarJMP(CWOffset(0x98D04), &ASMOnTalkToCreature);
	ASMOnTalkToCreature_jmpback = CWOffset(0x98D15);
	ASMOnTalkToCreature_jmpback2 = CWOffset(0x98ED2);
	ASMOnTalkToCreature_bail= CWOffset(0x988C1);
}

