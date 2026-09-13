# Feature: World Generation Updates (ID: 6)

The **World Generation** sub-mod (`cubeforge-world-gen.dll`) enhances terrain variety and character starting experiences by un-restricting spawn biomes and introducing macro-scale island distributions using Simplex Noise.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-world-gen.dll` (`src/mods/world_gen/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-world-gen.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Features & Changes

### 🌍 Un-restricted Spawn Regions
- In vanilla Cube World, newly created characters are restricted to standard temperate starting lands.
- With WorldGenMod enabled, `cube::Game::SetRestrictedSpawnRegions(false)` allows newly created characters to spawn in any standard temperature biome (hills, ocean archipelagos, mountains, dense woodlands, etc.).

### 🏝️ Cohesive Biome Island Generation
- Generates larger, distinct macro-islands sharing coherent biome types using a custom 2D/3D **Simplex Noise algorithm** (`SimplexNoise.cpp`).
- Preserves full backward-compatibility with existing save files while generating richer unexplored terrain.

### 🏛️ Building & Structure Overrides
- Custom structure distribution rules hooked via native detours ensuring balanced village and ruin placements.

---

## 3. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 6 1` | Enables World Generation enhancements. |
| `/cubeforge mod 6 0` | Disables World Generation enhancements. |
| `/mod 6 1` / `/mod 6 0` | Legacy alias for toggling World Generation. |

---

## 4. Technical Implementation

- **Class**: [`WorldGenMod`](file:///d:/Projects/CubeMegaMod/src/mods/world_gen/WorldGenMod.h) (`src/mods/world_gen/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Noise Generator**: `src/mods/world_gen/SimplexNoise.cpp`
- **Native MASM Detours** (`src/mods/world_gen/asm/hooks_world_gen.asm`):
  - Intercepts zone biome calculations via ASM detour at offset `0x265F06`
  - Overrides structure placement and building type logic

