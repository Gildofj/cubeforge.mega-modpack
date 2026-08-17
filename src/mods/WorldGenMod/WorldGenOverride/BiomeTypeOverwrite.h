/*
* Hook for overwriting the world gen, by modifying the getbiome type function.
*/

#pragma once

#include "../Noise/SimplexNoise.h"
#include "cwsdk.h"
#include <cmath>
#include <algorithm>

inline int DistanceSquared(IntVector2 p1, IntVector2 p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}

inline int GetBiomeType(int x, int y)
{
	float dx = std::floor(x / 7.f);
	float dy = std::floor(y / 7.f);

	float n = (SimplexNoise::noise(dx, dy) + 1.0f) * 0.5f; // [0.0, 1.0]
	int biome = static_cast<int>(std::floor(n * 6.0f));
	return std::clamp(biome, 0, 5);
}

inline int GetHeight(int x, int y)
{
	const float MULT = std::sqrt(3.f * 3.f + 3.f * 3.f);
	int dx = std::abs((std::abs(x) % 7) - 3);
	int dy = std::abs((std::abs(y) % 7) - 3);
	float dist = static_cast<float>(std::sqrt(DistanceSquared(IntVector2(dx, dy), IntVector2(0, 0))));
	float multiplier = (MULT - dist) / MULT;
	float height = multiplier * (2.f + SimplexNoise::noise(static_cast<float>(x), static_cast<float>(y))) / 3.f;

	if (height > 0.5f)
	{
		return 2;
	}
	else if (height > 0.25f)
	{
		return 1;
	}
	return 0;
}

extern "C" int GetRegionType(int x, int y) {
	using namespace cube;
	static const int REGION_TYPES[6][2] = {
		{(int)Zone::RegionType::Hills, (int)Zone::RegionType::Mountains},
		{(int)Zone::RegionType::Wetlands, (int)Zone::RegionType::Jungle},
		{(int)Zone::RegionType::Savannah, (int)Zone::RegionType::Woodlands},
		{(int)Zone::RegionType::Snowlands, (int)Zone::RegionType::SnowForest},
		{(int)Zone::RegionType::Desert, (int)Zone::RegionType::Firelands},
		{(int)Zone::RegionType::Deadlands, (int)Zone::RegionType::Darkwoods},
	};

	int height = GetHeight(x, y);
	if (height <= 0)
	{
		return (int)Zone::RegionType::Ocean;
	}

	int biomeIdx = std::clamp(GetBiomeType(x, y), 0, 5);
	int heightIdx = std::clamp(height - 1, 0, 1);
	return REGION_TYPES[biomeIdx][heightIdx];
}

extern "C" void ASMUpdateGetBiomeType();

void WorldGenOverrideIntialize()
{
	WriteFarJMP(CWOffset(0x2B909A), (void*)&ASMUpdateGetBiomeType);
}