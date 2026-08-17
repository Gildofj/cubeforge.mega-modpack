#pragma once

#include <Windows.h>
#include <psapi.h>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include "../main.h"

#define CUBE_EXE_NAME "cubeworld.exe"

class MemoryProtectGuard
{
private:
	void* m_address;
	size_t m_size;
	DWORD m_oldProtect;
	bool m_success;

public:
	MemoryProtectGuard(void* address, size_t size, DWORD newProtect = PAGE_EXECUTE_READWRITE)
		: m_address(address), m_size(size), m_oldProtect(0), m_success(false)
	{
		if (m_address && m_size > 0)
		{
			m_success = (VirtualProtect(m_address, m_size, newProtect, &m_oldProtect) != 0);
		}
	}

	~MemoryProtectGuard()
	{
		if (m_success && m_address && m_size > 0)
		{
			DWORD temp;
			VirtualProtect(m_address, m_size, m_oldProtect, &temp);
		}
	}

	bool IsValid() const { return m_success; }
};

class MemoryHelper
{
public:
	static MODULEINFO GetModuleInfo(HMODULE module_handle)
	{
		MODULEINFO module_info = { 0 };
		if (module_handle)
		{
			GetModuleInformation(GetCurrentProcess(), module_handle, &module_info, sizeof(MODULEINFO));
		}
		return module_info;
	}

	static MODULEINFO GetModuleInfo(const char* module_name)
	{
		return GetModuleInfo(GetModuleHandleA(module_name));
	}

	static uint64_t GetCubeBase()
	{
		static auto cube_info = GetModuleInfo(CUBE_EXE_NAME);
		return (uint64_t)cube_info.lpBaseOfDll;
	}

	/**
	 * Searches the cubeworld module for any instance of a specified string, and replaces it in memory
	 * @param search
	 * @param replace
	 */
	static void FindAndReplaceString(const std::wstring& search, const std::wstring& replace)
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

	// Default to cubeworld module
	static uint64_t FindPattern(const std::string& pattern, bool get_end = false)
	{
		return FindPattern(CUBE_EXE_NAME, pattern, get_end);
	}

	static uint64_t FindPattern(const std::string& module_name, const std::string& pattern, bool get_end = false)
	{
		std::vector<uint8_t> pattern_bytes;
		std::vector<bool> pattern_mask; // true if must match, false if wildcard

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

	static void PatchMemory(void* dst, const void* src, size_t size)
	{
		if (!dst || !src || size == 0) return;

		MemoryProtectGuard guard(dst, size, PAGE_EXECUTE_READWRITE);
		if (guard.IsValid())
		{
			memcpy(dst, src, size);
		}
	}

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

