#pragma once

#include <Windows.h>
#include <psapi.h>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

#define CUBE_EXE_NAME "cubeworld.exe"

class MemoryProtectGuard
{
private:
	void* m_address;
	size_t m_size;
	DWORD m_oldProtect;
	bool m_success;

public:
	MemoryProtectGuard(void* address, size_t size, DWORD newProtect = PAGE_EXECUTE_READWRITE);
	~MemoryProtectGuard();
	bool IsValid() const { return m_success; }
};

class MemoryHelper
{
public:
	static MODULEINFO GetModuleInfo(HMODULE module_handle);
	static MODULEINFO GetModuleInfo(const char* module_name);
	static uint64_t GetCubeBase();

	static void PatchMemory(void* dst, const void* src, size_t size);
	static void WriteByte(void* dst, unsigned char val);
	static void WriteFarJMP(void* source, void* destination);

	static void FindAndReplaceString(const std::wstring& search, const std::wstring& replace);
	static uint64_t FindPattern(const std::string& pattern, bool get_end = false);
	static uint64_t FindPattern(const std::string& module_name, const std::string& pattern, bool get_end = false);

	template<class T>
	static void PatchMemory(uint64_t dst, const T& src)
	{
		PatchMemory((void*)dst, &src, sizeof(T));
	}

	template<class T>
	static void PatchMemory(void* dst, const T& src)
	{
		PatchMemory(dst, &src, sizeof(T));
	}
};
