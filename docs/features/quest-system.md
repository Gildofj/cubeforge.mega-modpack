# Feature: Quest System (ID: 10)

The **Quest System** sub-mod (`cubeforge-quest-system.dll`) introduces procedural, dynamic quests given by town and roaming NPCs, complete with kill tracking, inventory-based quest scrolls, and automatic completion rewards.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-quest-system.dll` (`src/mods/quest_system/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-quest-system.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Mechanics & Quest Acquisition

- **Trigger**: Talking to eligible NPCs (NPCs whose ID passes the `creature->id % 5 == 0` check).
- **Quest Item Drop**: The NPC drops a Quest Scroll item into the world, which is stored in the player's **Ingredients** inventory tab.
- **Quest Rarity**: Quests spawn with varying star rarities (`0` to `4`), scaling required kills and reward payouts.

### Supported Monster Targets

The procedural quest generator can target over 30 distinct mob species:
- *Domestic/Farm Animals*: Cow, Sheep, Chicken, Pig, Alpaca, Horse, Bunny
- *Canines/Creatures*: Collie, Terrier, Scottish Terrier, Skull Bull, Turtle, Ogre, Rockling
- *Insects/Flyers*: Fly, Hornets, Crow, Bat, Bark Beetle, Fire Beetle, Snout Beetle, Lemon Beetle
- *Runners & Plantlings*: Plain Runner, Snow Runner, Desert Runner, Radishling, Onionling, Desert Onionling
- *Monsters*: Imp, Spitter, Mole, Biter, Mana Deer

---

## 3. Tracking & Completion Lifecycle

1. **Kill Interception**: When a player (or their pet) kills a creature, `OnCreatureDeath` inspects all active quest scrolls in the player's Ingredients tab.
2. **Progress Increment**: If the killed creature's race matches the quest target, `quest->IncreaseProgress()` advances the counter.
3. **Completion & Reward**: Once target kills are fulfilled, `quest->Complete()` grants gold/XP/items and automatically removes the completed scroll from the inventory.

---

## 4. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 10 1` | Enables Quest System. |
| `/cubeforge mod 10 0` | Disables Quest System. |
| `/mod 10 1` / `/mod 10 0` | Legacy alias for toggling Quest System. |

---

## 5. Technical Implementation

- **Class**: [`QuestMod`](file:///d:/Projects/CubeMegaMod/src/mods/quest_system/QuestMod.h) (`src/mods/quest_system/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Speech Map Injections**: Registers custom speech string identifiers (`QuestKill`, `QuestGather`, `QuestDeliver`, `QuestTalk`) at offset indices `15..18`.
- **String Patches**: Replaces `"Formula: "` with `"[Quest]: "` in memory via `cubeforge::memory::MemoryHelper::FindAndReplaceString`.
- **Native MASM Detours & Hooks** (`src/mods/quest_system/asm/hooks_quest_system.asm`):
  - Dynamic quest scroll name and description formatting
  - Creature death listener capturing all mob defeats across the world

