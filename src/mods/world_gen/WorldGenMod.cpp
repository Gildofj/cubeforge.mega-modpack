#include "WorldGenMod.h"
#include "SimplexNoise.h"
#include "memory/MemoryHelper.h"
#include <cmath>
#include <algorithm>

// Hooks ASM
extern "C" {
	void ASMUpdateGetBiomeType();
	void ASMBuildingTypeOverride();
}

static inline int DistanceSquared(IntVector2 p1, IntVector2 p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}

static inline int GetBiomeType(int x, int y)
{
	float dx = std::floor(x / 7.f);
	float dy = std::floor(y / 7.f);

	float n = (SimplexNoise::noise(dx, dy) + 1.0f) * 0.5f;
	int biome = static_cast<int>(std::floor(n * 6.0f));
	return std::clamp(biome, 0, 5);
}

static inline int GetHeight(int x, int y)
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

extern "C" int GetRegionType(int x, int y)
{
	using namespace cube;
	static const int REGION_TYPES[6][2] = {
		{ static_cast<int>(Zone::RegionType::Hills), static_cast<int>(Zone::RegionType::Mountains) },
		{ static_cast<int>(Zone::RegionType::Wetlands), static_cast<int>(Zone::RegionType::Jungle) },
		{ static_cast<int>(Zone::RegionType::Savannah), static_cast<int>(Zone::RegionType::Woodlands) },
		{ static_cast<int>(Zone::RegionType::Snowlands), static_cast<int>(Zone::RegionType::SnowForest) },
		{ static_cast<int>(Zone::RegionType::Desert), static_cast<int>(Zone::RegionType::Firelands) },
		{ static_cast<int>(Zone::RegionType::Deadlands), static_cast<int>(Zone::RegionType::Darkwoods) },
	};

	int height = GetHeight(x, y);
	if (height <= 0)
	{
		return static_cast<int>(Zone::RegionType::Ocean);
	}

	int biomeIdx = std::clamp(GetBiomeType(x, y), 0, 5);
	int heightIdx = std::clamp(height - 1, 0, 1);
	return REGION_TYPES[biomeIdx][heightIdx];
}

constexpr int ZONE_TYPES = 5;

static inline int Convert(long long coord)
{
	int n = static_cast<int>(coord >> 32);
	int nn = static_cast<int>(coord & 0xFFFFFFFF);
	return n ^ nn;
}

static inline int GetBuildingZoneType(long long x, long long y)
{
	int size = 100;

	int xx = Convert(x) / size;
	int yy = Convert(y) / size;

	if ((xx & 1) != 0) xx += 1;
	if ((yy & 1) != 0) yy += 10;

	float n = (SimplexNoise::noise(static_cast<float>(xx), static_cast<float>(yy)) + 1.f) * 0.5f;
	int zone = static_cast<int>(n * ZONE_TYPES);
	return std::clamp(zone, 0, ZONE_TYPES - 1);
}

extern "C" int GetBuildingType(cube::World* world, long long x, long long y, IntVector2* intVector2)
{
	(void)world;
	int zone = GetBuildingZoneType(x, y);
	switch (zone)
	{
	case 0:
	{
		static const int ARR[] = { 22, 23, 24, 24, 25, 53, 0, 35, 36, 37, 40, 43, 77 };
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
			if (noise > 0.5f) return 53;
			if (noise > -0.5f) return 16;
			return 17;
		}

		if (res < 0.0f) return 4;
		return 8;
	}
	case 2: // lore lands
		return 71 + static_cast<int>(std::round(2.f * SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y)))));
	case 3: // floating islands with fields
	{
		if (SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y))) > 0.8f) return 60;
		static const int ARR[] = { 27, 28, 29, 30, 31, 32, 33 };
		int size = sizeof(ARR) / sizeof(*ARR);
		int res = static_cast<int>((SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y))) + 1.f) * size) % size;
		if (res < 0) res = 0;
		return ARR[res];
	}
	case 4: // mage city
	{
		if (SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y))) > 0.8f) return 52;
		static const int ARR[] = { 46 };
		int size = sizeof(ARR) / sizeof(*ARR);
		int res = static_cast<int>((SimplexNoise::noise(static_cast<float>(Convert(x)), static_cast<float>(Convert(y))) + 1.f) * size) % size;
		if (res < 0) res = 0;
		return ARR[res];
	}
	default:
		return 0;
	}
}

WorldGenMod::WorldGenMod()
	: BaseMod("World Gen Mod", "cubeforge-world-gen", 6, { 1, 1, 0 })
{
}

void WorldGenMod::Initialize()
{
	cube::Game::SetRestrictedSpawnRegions(false);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2B909A), (void*)&ASMUpdateGetBiomeType);
	MemoryHelper::WriteFarJMP((void*)CWOffset(0x2D53D0), (void*)&ASMBuildingTypeOverride);
}

#ifndef MODPACK_BUILD
EXPORT GenericMod* MakeMod() {
	return new WorldGenMod();
}
#endif
