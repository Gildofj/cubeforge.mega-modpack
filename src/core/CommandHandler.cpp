#include "CommandHandler.h"
#include "creature/Creature.h"
#include "creature/CreatureFactory.h"
#include <cwchar>
#include <algorithm>

static std::wstring ToLower(const std::wstring& str)
{
	std::wstring lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::towlower);
	return lower;
}

bool CommandHandler::ProcessCommand(cube::Game* game, const std::wstring& message, std::vector<BaseMod*>& mods)
{
	if (!game || message.empty()) return false;

	const wchar_t* msg = message.c_str();

	// 1. Comando principal: /cubeforge status
	if (message == L"/cubeforge status" || message == L"/cubeforge")
	{
		PrintStatus(game, mods);
		return true;
	}

	// 2. Comando principal: /cubeforge help
	if (message == L"/cubeforge help")
	{
		PrintHelp(game);
		return true;
	}

	// 3. Comando: /cubeforge list
	if (message == L"/cubeforge list")
	{
		game->PrintMessage(L"[CubeForge] Modulos Registrados:\n", 0, 255, 255);
		for (BaseMod* mod : mods)
		{
			if (!mod) continue;
			wchar_t buf[256];
			swprintf_s(buf, 256, L"  - [%d] %S (%S) -> %s\n",
				mod->m_ID,
				mod->m_Name.c_str(),
				mod->m_Version.ToString().c_str(),
				mod->m_Enabled ? L"ATIVADO" : L"DESATIVADO");
			game->PrintMessage(buf, mod->m_Enabled ? 100 : 200, mod->m_Enabled ? 255 : 100, 100);
		}
		return true;
	}

	// 4. Comando: /cubeforge mod <ID> <1/0> ou /mod <ID> <1/0> (Legado)
	int modID, val;
	if (swscanf_s(msg, L"/cubeforge mod %d %d", &modID, &val) == 2 ||
		swscanf_s(msg, L"/mod %d %d", &modID, &val) == 2)
	{
		for (BaseMod* mod : mods)
		{
			if (mod && mod->m_ID == modID)
			{
				mod->m_Enabled = (val != 0);
				wchar_t buf[256];
				swprintf_s(buf, 256, L"[CubeForge] %S foi %s\n",
					mod->m_Name.c_str(),
					mod->m_Enabled ? L"ATIVADO" : L"DESATIVADO");
				game->PrintMessage(buf, mod->m_Enabled ? 100 : 255, mod->m_Enabled ? 255 : 100, 100);
			}
		}
		return true;
	}

	// 5. Comandos de spawn e animação para testes
	int classID;
	if (swscanf_s(msg, L"/cubeforge class %d", &classID) == 1 ||
		swscanf_s(msg, L"/class %d", &classID) == 1)
	{
		cube::Creature* player = game->GetPlayer();
		if (player)
		{
			// Spawn NPC com classe para testes
			cube::Creature* creature = cube::CreatureFactory::SpawnCreature(
				player->entity_data.position,
				player->entity_data.current_region,
				304,
				static_cast<int>(cube::Enums::EntityBehaviour::NPC),
				1
			);
			if (creature)
			{
				creature->entity_data.appearance.flags2 |= 1 << static_cast<int>(cube::Enums::AppearanceModifiers::NeededForGemTrader);
				creature->entity_data.classType = classID;
				creature->entity_data.specialization = 1;
				game->PrintMessage(L"[CubeForge] NPC de teste spawnado.\n", 100, 255, 100);
			}
		}
		return true;
	}

	int animID;
	if (swscanf_s(msg, L"/cubeforge anim %d", &animID) == 1 ||
		swscanf_s(msg, L"/t %d", &animID) == 1)
	{
		cube::Creature* player = game->GetPlayer();
		if (player)
		{
			player->animation_state.current_animation_state_timer = 0.f;
			player->animation_state.current_animation_state_id = animID;
			game->PrintMessage(L"[CubeForge] Animacao aplicada.\n", 100, 255, 100);
		}
		return true;
	}

	// Repassa para os mods individuais
	std::wstring modMsg = message;
	for (BaseMod* mod : mods)
	{
		if (mod && mod->m_Enabled)
		{
			if (mod->OnChat(&modMsg))
			{
				return true;
			}
		}
	}

	return false;
}

void CommandHandler::PrintHelp(cube::Game* game)
{
	if (!game) return;
	game->PrintMessage(L"================ [Comandos CubeForge] ================\n", 0, 255, 255);
	game->PrintMessage(L"/cubeforge status             - Exibe status dos mods\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge list               - Lista todos os modulos\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge mod <id> <1/0>     - Ativa/desativa modulo por ID\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge combat doubletap <1/0> - Toggle esquiva double-tap\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge sea autogold <1/0>     - Toggle auto ouro mergulho\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge class <id>         - Spawn NPC teste de classe\n", 255, 255, 255);
	game->PrintMessage(L"/cubeforge anim <id>          - Testa animacao do jogador\n", 255, 255, 255);
	game->PrintMessage(L"======================================================\n", 0, 255, 255);
}

void CommandHandler::PrintStatus(cube::Game* game, const std::vector<BaseMod*>& mods)
{
	if (!game) return;
	game->PrintMessage(L"[CubeForge Framework] Modulos Carregados:\n", 100, 255, 100);
	int activeCount = 0;
	for (BaseMod* mod : mods)
	{
		if (mod && mod->m_Enabled) activeCount++;
	}
	wchar_t summary[128];
	swprintf_s(summary, 128, L"Total: %zu modulos (%d ativos)\n", mods.size(), activeCount);
	game->PrintMessage(summary, 255, 255, 255);
	game->PrintMessage(L"Digite /cubeforge list ou /cubeforge help para mais opcoes.\n", 180, 180, 180);
}
