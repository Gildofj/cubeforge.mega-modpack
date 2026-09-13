#include "MemoryHelper.h"
#include <cstring>
#include <algorithm>

MemoryProtectGuard::MemoryProtectGuard(void* address, size_t size, DWORD newProtect)
	: m_address(address), m_size(size), m_oldProtect(0), m_success(false)
{
	if (m_address && m_size > 0)
	{
		m_success = (VirtualProtect(m_address, m_size, newProtect, &m_oldProtect) != 0);
	}
}

MemoryProtectGuard::~MemoryProtectGuard()
{
	if (m_success && m_address && m_size > 0)
	{
		DWORD temp;
		VirtualProtect(m_address, m_size, m_oldProtect, &temp);
	}
}

MODULEINFO MemoryHelper::GetModuleInfo(HMODULE module_handle)
{
	MODULEINFO module_info = { 0 };
	if (module_handle)
	{
		GetModuleInformation(GetCurrentProcess(), module_handle, &module_info, sizeof(MODULEINFO));
	}
	return module_info;
}

MODULEINFO MemoryHelper::GetModuleInfo(const char* module_name)
{
	return GetModuleInfo(GetModuleHandleA(module_name));
}

uint64_t MemoryHelper::GetCubeBase()
{
	static auto cube_info = GetModuleInfo(CUBE_EXE_NAME);
	return (uint64_t)cube_info.lpBaseOfDll;
}

void MemoryHelper::PatchMemory(void* dst, const void* src, size_t size)
{
	if (!dst || !src || size == 0) return;

	MemoryProtectGuard guard(dst, size, PAGE_EXECUTE_READWRITE);
	if (guard.IsValid())
	{
		memcpy(dst, src, size);
	}
}

void MemoryHelper::WriteByte(void* dst, unsigned char val)
{
	PatchMemory(dst, &val, 1);
}

void MemoryHelper::WriteFarJMP(void* source, void* destination)
{
	if (!source || !destination) return;

	// 14-byte absolute jump in x64:
	// FF 25 00 00 00 00 [64-bit absolute address]
	unsigned char jmpBytes[14] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	memcpy(&jmpBytes[6], &destination, sizeof(void*));
	PatchMemory(source, jmpBytes, sizeof(jmpBytes));
}

void MemoryHelper::FindAndReplaceString(const std::wstring& search, const std::wstring& replace)
{
	HMODULE module_handle = GetModuleHandleA(CUBE_EXE_NAME);
	if (!module_handle) return;

	MODULEINFO module_info = GetModuleInfo(module_handle);
	uint64_t base_address = (uint64_t)module_info.lpBaseOfDll;
	uint64_t module_size = module_info.SizeOfImage;

	const wchar_t* search_cstr = search.c_str();
	size_t search_len = search.size();

	if (module_size < search_len * sizeof(wchar_t)) return;

	for (uint64_t i = 0; i <= module_size - (search_len * sizeof(wchar_t)); i++)
	{
		const wchar_t* check_addr = (const wchar_t*)(base_address + i);

		bool found = true;
		for (size_t b = 0; b < search_len; b++)
		{
			if (search_cstr[b] != check_addr[b])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			PatchMemory((void*)check_addr, (const void*)replace.c_str(), (replace.size() + 1) * sizeof(wchar_t));
		}
	}
}

uint64_t MemoryHelper::FindPattern(const std::string& pattern, bool get_end)
{
	return FindPattern(CUBE_EXE_NAME, pattern, get_end);
}

uint64_t MemoryHelper::FindPattern(const std::string& module_name, const std::string& pattern, bool get_end)
{
	std::vector<uint8_t> pattern_bytes;
	std::vector<bool> pattern_mask;

	std::istringstream iss(pattern);
	for (std::string s; iss >> s;)
	{
		if (s == "?" || s == "??")
		{
			pattern_bytes.push_back(0);
			pattern_mask.push_back(false);
		}
		else
		{
			pattern_bytes.push_back(static_cast<uint8_t>(std::stoul(s, nullptr, 16)));
			pattern_mask.push_back(true);
		}
	}

	if (pattern_bytes.empty()) return 0;

	HMODULE module_handle = GetModuleHandleA(module_name.c_str());
	if (!module_handle) return 0;

	MODULEINFO module_info = GetModuleInfo(module_handle);
	uint64_t base_address = (uint64_t)module_info.lpBaseOfDll;
	uint64_t module_size = module_info.SizeOfImage;

	if (module_size < pattern_bytes.size()) return 0;

	size_t pat_len = pattern_bytes.size();
	for (uint64_t i = 0; i <= module_size - pat_len; i++)
	{
		uint64_t check_addr = base_address + i;

		bool found = true;
		for (size_t b = 0; b < pat_len; b++)
		{
			if (pattern_mask[b] && pattern_bytes[b] != *(const uint8_t*)(check_addr + b))
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return get_end ? (check_addr + pat_len) : check_addr;
		}
	}

	return 0;
}
