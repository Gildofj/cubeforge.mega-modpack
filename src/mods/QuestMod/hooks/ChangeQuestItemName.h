/*
* Hook for letting smithy interaction open the inventory and adaption widget.
*/
#include <string>
#include "cwsdk.h"
#include "../../../cwsdk-extension.h"

extern "C" int OnGetItemName(cube::Speech* speech, cube::Item* item, cube::Item* copy)
{
	const static int ID_OFFSET = 15;
	if (item->category == 2)
	{
		*copy = cube::Item(1, ID_OFFSET + (int)((cube::Quest*)item)->GetType());
		return 1;
	}
	return 0;
}

GETTER_VAR(void*, ASMOnGetItemName_jmpback);
GETTER_VAR(void*, ASMOnGetItemName_bail);
extern "C" void ASMOnGetItemName();

// Todo: Call for every mod
void SetupOnGetItemName()
{
	WriteFarJMP(CWOffset(0x1628E4), &ASMOnGetItemName);
	ASMOnGetItemName_jmpback = CWOffset(0x1628F5);
	ASMOnGetItemName_bail = CWOffset(0x1629A4);
}

