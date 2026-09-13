#pragma once

#include <string>
#include <vector>
#include "BaseMod.h"

class CommandHandler
{
public:
	static bool ProcessCommand(cube::Game* game, const std::wstring& message, std::vector<BaseMod*>& mods);
	static void PrintHelp(cube::Game* game);
	static void PrintStatus(cube::Game* game, const std::vector<BaseMod*>& mods);
};
