/*
* This is a header file for including all the hooks code.
*/
#pragma once
#include <cwsdk.h>

// Compiler portability definitions
#ifdef _MSC_VER
#ifndef __attribute__
#define __attribute__(x)
#endif
#define no_optimize
#else
#define no_optimize __attribute__((optimize("O0")))
#endif

// Linkage for MASM-referenced global jump pointers
#define GETTER_VAR(vartype, varname) extern "C" vartype varname;

static void Popup(const char* title, const char* msg) {
	// Non-blocking log or debug output
	OutputDebugStringA(msg);
}

namespace hook
{
	enum class HookEvent
	{
		LoreInteraction = 0
	};

	struct HookEventData
	{
		HookEvent type;
		int data;
	};

	// Initializes all hooks
	void InitializeAll(std::vector<HookEventData>* hookEvents);
}