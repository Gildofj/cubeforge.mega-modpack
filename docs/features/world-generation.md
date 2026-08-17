# Feature: World Generation Updates (ID: 6)

The **World Generation Mod** enhances terrain variety and character starting experiences by un-restricting spawn biomes and introducing macro-scale island distributions using Simplex Noise.

---

## 1. Features & Changes

### 🌍 Un-restricted Spawn Regions
- In vanilla Cube World, newly created characters are restricted to standard temperate starting lands.
- With WorldGenMod enabled, `cube::Game::SetRestrictedSpawnRegions(false)` allows newly created characters to spawn in any standard temperature biome (hills, ocean archipelagos, mountains, dense woodlands, etc.).

### 🏝️ Cohesive Biome Island Generation
- Generates larger, distinct macro-islands sharing coherent biome types using a custom 2D/3D **Simplex Noise algorithm** (`SimplexNoise.cpp`).
- Preserves full backward-compatibility with existing save files while generating richer unexplored terrain.

### 🏛️ Building & Structure Overrides
- Custom structure distribution rules hooked via `BuildingTypeOverwrite.h` ensuring balanced village and ruin placements.

---

## 2. Technical Implementation

- **Class**: `WorldGenMod` (`src/mods/WorldGenMod/`)
- **Noise Generator**: `src/mods/WorldGenMod/Noise/SimplexNoise.cpp`
- **Hooks**:
  - `BiomeTypeOverwrite.h` intercepts zone biome calculations via ASM detour at offset `0x265F06`.
  - `BuildingTypeOverwrite.h` hooks structure placement logic.
