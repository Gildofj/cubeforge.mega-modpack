#include "FileParser.h"
#include <fstream>
#include <windows.h>
#include "cwsdk.h"

#include "../../CubeMod.h"

struct ModSetting
{
	int ID;
	bool enabled;
};

static ModSetting ReadModSetting(std::ifstream* file)
{
	ModSetting setting = { 0, false };
	if (file && file->good()) {
		file->read(reinterpret_cast<char*>(&setting), sizeof(setting));
	}
	return setting;
}

void cube::ApplySettings(std::vector<CubeMod*>* mods)
{
	if (!mods) {
		return;
	}

	char fileName[256] = { 0 };
	CreateDirectoryA("Mods", NULL);
	CreateDirectoryA(SAVE_FOLDER, NULL);
	snprintf(fileName, sizeof(fileName), "%s\\%s", SAVE_FOLDER, SAVE_FILE_NAME);

	std::ifstream file(fileName, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	int count = 0;
	file.read(reinterpret_cast<char*>(&count), sizeof(count));
	if (!file.good() || count <= 0 || count > 4096) {
		file.close();
		return;
	}

	std::vector<ModSetting> settings;
	settings.reserve(static_cast<size_t>(count));
	for (int i = 0; i < count && file.good(); i++)
	{
		settings.push_back(ReadModSetting(&file));
	}
	file.close();

	// Apply read settings to existing mods
	for (CubeMod* mod : *mods)
	{
		if (!mod) continue;
		for (const ModSetting& setting : settings)
		{
			if (setting.ID == mod->m_ID)
			{
				mod->m_Enabled = setting.enabled;
				break;
			}
		}
	}
}

void cube::SaveSettings(std::vector<CubeMod*>* mods)
{
	if (!mods) {
		return;
	}

	char fileName[256] = { 0 };
	CreateDirectoryA("Mods", NULL);
	CreateDirectoryA(SAVE_FOLDER, NULL);
	snprintf(fileName, sizeof(fileName), "%s\\%s", SAVE_FOLDER, SAVE_FILE_NAME);

	std::ofstream file(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.is_open()) {
		return;
	}

	int size = static_cast<int>(mods->size());
	file.write(reinterpret_cast<const char*>(&size), sizeof(size));
	for (CubeMod* mod : *mods)
	{
		if (!mod) continue;
		ModSetting setting = { mod->m_ID, mod->m_Enabled };
		file.write(reinterpret_cast<const char*>(&setting), sizeof(setting));
	}

	file.close();
}

