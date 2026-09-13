# Feature: Sea Exploration (ID: 1)

The **Sea Exploration** sub-mod (`cubeforge-sea-exploration.dll`) breathes life into Cube World's vast ocean biomes by introducing deep diving oxygen systems, custom underwater treasure chests, hostile aquatic mob encounters, and rare deep-sea boss spawns.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-sea-exploration.dll` (`src/mods/sea_exploration/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-sea-exploration.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Overview & Mechanics

### Underwater Diving & Oxygen
- **Breathing System**: When submerged under water, stamina is normally consumed. With Sea Exploration enabled, **10 Gold** is automatically consumed every 10 seconds to grant the player extended underwater breathing.
- **Auto Gold Toggle**: Automatic gold consumption can be configured via `/cubeforge sea autogold <1/0>`.
- **Mob Encounters**: Hostile sea creatures periodically spawn around diving players.

---

## 3. Underwater Chests & Loot Tables

While exploring underwater, special treasure chests spawn every 60 seconds. Chests are categorized by race/type (indices 0–3):

| Chest Type | Visual Model | Drop Chances & Rewards |
| :--- | :--- | :--- |
| **Normal / Wood Chest** (`type 0`) | Standard Wood Chest | • 10 Gold (100%)<br>• 1x Gear item (75%)<br>• 1x Consumable (50%) |
| **Skull Chest** (`type 1`) | Skull Chest | • 5 to 35 Gold (100%)<br>• 1–2x 4–8 Star Gear items (100%)<br>• Rare Artifact (5% chance) |
| **Obsidian Chest** (`type 2`) | Dark Obsidian Chest | • 1x Guaranteed Artifact (100%) |
| **Bone Chest** (`type 3`) | Bone Chest | • 1x Guaranteed Pet (100%, includes rare/unobtainable pets) |

---

## 4. Deep-Sea Bosses

Random deep-sea bosses with massive health pools (e.g. 500,000+ HP) can spawn near deep ocean trenches. These encounters provide high-end challenges for players equipped with 5+ star gear.

---

## 5. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 1 1` | Enables Sea Exploration. |
| `/cubeforge mod 1 0` | Disables Sea Exploration. |
| `/cubeforge sea autogold 1` | Enables automatic gold consumption for diving oxygen. |
| `/cubeforge sea autogold 0` | Disables automatic gold consumption. |
| `/enable autogoldusage` / `/disable autogoldusage` | Legacy aliases for auto-gold toggling. |

---

## 6. Technical Implementation

- **Class**: [`SeaExplorationMod`](file:///d:/Projects/CubeMegaMod/src/mods/sea_exploration/SeaExplorationMod.h) (`src/mods/sea_exploration/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Event Driver**: `cube::DivingEvent` (`src/mods/sea_exploration/event/DivingEvent.cpp`)
- **Native MASM Detours & Hooks** (`src/mods/sea_exploration/asm/hooks_sea_exploration.asm`):
  - Intercepts entity interaction packets
  - Maps race offsets (`race - 181`) to chest loot dispatchers

