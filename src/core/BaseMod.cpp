#include "BaseMod.h"
#include <cstdio>

void BaseMod::Save(const void* data, size_t size)
{
	if (!data || size == 0 || !m_FileName) {
		return;
	}

	char fileName[MAX_PATH] = { 0 };
	CreateDirectoryA("Mods", NULL);
	snprintf(fileName, sizeof(fileName), "Mods\\%s.sav", m_FileName);

	std::ofstream file(fileName, std::ios::out | std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
	file.close();
}

void BaseMod::Load(void* data, size_t size)
{
	if (!data || size == 0 || !m_FileName) {
		return;
	}

	char fileName[MAX_PATH] = { 0 };
	CreateDirectoryA("Mods", NULL);
	snprintf(fileName, sizeof(fileName), "Mods\\%s.sav", m_FileName);

	std::ifstream file(fileName, std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		return;
	}

	file.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
	file.close();
}
