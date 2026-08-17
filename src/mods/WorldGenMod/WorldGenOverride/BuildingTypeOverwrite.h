/*
* Hook for overwriting the world gen, by modifying the getbiome type function.
*/
#pragma once

#include "../Noise/SimplexNoise.h"
#include "cwsdk.h"
#include <cmath>
#include <algorithm>

constexpr int ZONE_TYPES = 5;

inline int Convert(long long coord)
{
	int n = static_cast<int>(coord >> 32);
	int nn = static_cast<int>(coord & 0xFFFFFFFF);
	int nnn = n ^ nn;
	return nnn;
}

inline int GetZoneSize(long long x, long long y)
{
	(void)x;
	(void)y;
	return 100;
}

inline int GetBuildingZoneType(long long x, long long y)
{
	int size = GetZoneSize(x, y);

	int xx = Convert(x) / size;
	int yy = Convert(y) / size;

	if ((xx & 1) != 0) xx += 1;
	if ((yy & 1) != 0) yy += 10;

	float n = (SimplexNoise::noise(static_cast<float>(xx), static_cast<float>(yy)) + 1.f) * 0.5f;
	int zone = static_cast<int>(n * ZONE_TYPES);
	return std::clamp(zone, 0, ZONE_TYPES - 1);
}

extern "C" int GetBuildingType(cube::World* world, long long x, long long y, IntVector2* intVector2) {
	(void)world;
	int zone = GetBuildingZoneType(x, y);
	switch (zone)
	{
	case 0:
	{
		static const int ARR[] = {
			22, 23, 24, 24, 25, 53, 0, 35, 36, 37, 40, 43, 77,
		};

		int size = sizeof(ARR) / sizeof(*ARR);
		float n = (SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y))) + 1.f) * 0.5f;
		int idx = static_cast<int>(n * size) % size;
		if (idx < 0) idx = 0;
		return ARR[idx];
	}
	case 1: // jungle
	{
		float res = SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y)));

		if (res > 0.9f) {
			if (!intVector2) return 17;
			float noise = SimplexNoise::noise(static_cast<float>(Convert(intVector2->x)), static_cast<float>(Convert(intVector2->y)));
			if (noise > 0.5f) {
				return 53;
			}

			if (noise > -0.5f)
			{
				return 16;
			}

			return 17;
		}

		if (res < 0.0f)
		{
			return 4;
		}

		return 8;
	}
		break;
	case 2: // lore lands
	{
		return 71 + std::round(2.f * SimplexNoise::noise((float)Convert(x), (float)Convert(y)));
	}
		break;
	case 3: // floating islands with flields on the ground
	{
		
		if (SimplexNoise::noise(Convert(x), Convert(y)) > 0.8f)
		{
			return 60;
		}

		int ARR[] = {
			27,
			28,
			29,
			30,
			31,
			32,
			33
		};

		int size = sizeof(ARR) / sizeof(*ARR);
		int res = ((int)((SimplexNoise::noise(Convert(x), Convert(y)) + 1.f) * size)) % size;
		return ARR[res];
	}
		break;
	case 4: // mage city
	{

		if (SimplexNoise::noise(Convert(x), Convert(y)) > 0.8f)
		{
			return 52;
		}

		int ARR[] = {
			46
		};

		int size = sizeof(ARR) / sizeof(*ARR);
		int res = ((int)((SimplexNoise::noise(Convert(x), Convert(y)) + 1.f) * size)) % size;
		return ARR[res];
	}
	break;
	default:
		return 0;
		break;
	}

	return 0;
}

__attribute__((naked)) void ASMBuildingTypeOverride() {
	asm(".intel_syntax \n"

		"sub rsp, 0x110 \n"
		"movups xmmword ptr [rsp + 0x10], xmm0 \n"
		"movups xmmword ptr [rsp + 0x20], xmm1 \n"
		"movups xmmword ptr [rsp + 0x30], xmm2 \n"
		"movups xmmword ptr [rsp + 0x40], xmm3 \n"
		"movups xmmword ptr [rsp + 0x50], xmm4 \n"
		"movups xmmword ptr [rsp + 0x60], xmm5 \n"
		"movups xmmword ptr [rsp + 0x70], xmm6 \n"
		"movups xmmword ptr [rsp + 0x80], xmm7 \n"
		"movups xmmword ptr [rsp + 0x90], xmm8 \n"
		"movups xmmword ptr [rsp + 0xA0], xmm9 \n"
		"movups xmmword ptr [rsp + 0xB0], xmm10 \n"
		"movups xmmword ptr [rsp + 0xC0], xmm11 \n"
		"movups xmmword ptr [rsp + 0xD0], xmm12 \n"
		"movups xmmword ptr [rsp + 0xE0], xmm13 \n"
		"movups xmmword ptr [rsp + 0xF0], xmm14 \n"
		"movups xmmword ptr [rsp + 0x100], xmm15 \n"

		PREPARE_STACK
		"call GetBuildingType \n"
		RESTORE_STACK

		"movups xmm0, xmmword ptr [rsp + 0x10] \n"
		"movups xmm1, xmmword ptr [rsp + 0x20] \n"
		"movups xmm2, xmmword ptr [rsp + 0x30] \n"
		"movups xmm3, xmmword ptr [rsp + 0x40] \n"
		"movups xmm4, xmmword ptr [rsp + 0x50] \n"
		"movups xmm5, xmmword ptr [rsp + 0x60] \n"
		"movups xmm6, xmmword ptr [rsp + 0x70] \n"
		"movups xmm7, xmmword ptr [rsp + 0x80] \n"
		"movups xmm8, xmmword ptr [rsp + 0x90] \n"
		"movups xmm9, xmmword ptr [rsp + 0xA0] \n"
		"movups xmm10, xmmword ptr [rsp + 0xB0] \n"
		"movups xmm11, xmmword ptr [rsp + 0xC0] \n"
		"movups xmm12, xmmword ptr [rsp + 0xD0] \n"
		"movups xmm13, xmmword ptr [rsp + 0xE0] \n"
		"movups xmm14, xmmword ptr [rsp + 0xF0] \n"
		"movups xmm15, xmmword ptr [rsp + 0x100] \n"

		"add rsp, 0x110 \n"

		"ret \n"

		".att_syntax \n"
	);
}

void BuildingTypeOverrideIntialize()
{
	WriteFarJMP(CWOffset(0x2D53D0), (void*)&ASMBuildingTypeOverride);
}