# Feature: Sea Exploration (ID: 1)

The **Sea Exploration Mod** breathes life into Cube World's vast ocean biomes by introducing deep diving oxygen systems, custom underwater treasure chests, hostile aquatic mob encounters, and rare deep-sea boss spawns.

---

## 1. Overview & Mechanics

### Underwater Diving & Oxygen
- **Breathing System**: When submerged under water, stamina is normally consumed. With Sea Exploration enabled, **10 Gold** is automatically consumed every 10 seconds to grant the player extended underwater breathing.
- **Auto Gold Toggle**: Automatic gold consumption can be configured via `/enable autogoldusage` and `/disable autogoldusage`.
- **Mob Encounters**: Hostile sea creatures periodically spawn around diving players.

---

## 2. Underwater Chests & Loot Tables

While exploring underwater, special treasure chests spawn every 60 seconds. Chests are categorized by race/type (indices 0–3):

| Chest Type | Visual Model | Drop Chances & Rewards |
| :--- | :--- | :--- |
| **Normal / Wood Chest** (`type 0`) | Standard Wood Chest | • 10 Gold (100%)<br>• 1x Gear item (75%)<br>• 1x Consumable (50%) |
| **Skull Chest** (`type 1`) | Skull Chest | • 5 to 35 Gold (100%)<br>• 1–2x 4–8 Star Gear items (100%)<br>• Rare Artifact (5% chance) |
| **Obsidian Chest** (`type 2`) | Dark Obsidian Chest | • 1x Guaranteed Artifact (100%) |
| **Bone Chest** (`type 3`) | Bone Chest | • 1x Guaranteed Pet (100%, includes rare/unobtainable pets) |

---

## 3. Deep-Sea Bosses
Random deep-sea bosses with massive health pools (e.g. 500,000+ HP) can spawn near deep ocean trenches. These encounters provide high-end challenges for players equipped with 5+ star gear.

---

## 4. Technical Implementation

- **Class**: `SeaExplorationMod` (`src/mods/SeaExplorationMod/`)
- **Event Driver**: `cube::DivingEvent` (`src/mods/SeaExplorationMod/event/DivingEvent.cpp`)
- **Hook**: `SetupChestInteractionHandler` (`src/hooks/ChestInteractionHandler.h`) intercepts entity interaction packets and maps race offsets (`race - 181`) to chest loot dispatchers.
