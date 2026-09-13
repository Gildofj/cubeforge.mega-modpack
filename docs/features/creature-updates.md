# Feature: Creature Updates (ID: 4)

The **Creature Updates** sub-mod (`cubeforge-creature-updates.dll`) balances creature stats, enhances the viability of companion pets, adjusts the difficulty of overtuned ranged mobs, and provides a smoother economic start for newly created characters.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-creature-updates.dll` (`src/mods/creature_updates/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-creature-updates.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Overview & Stat Adjustments

### 🐾 50% Pet Buff Multiplier
All stats belonging to friendly companion pets (`EntityBehaviour::Pet`) receive an automatic **+50% multiplier (`* 1.5f`)**:
- **Max HP & Armor**
- **Critical Strike Chance**
- **Attack Power & Spell Power**
- **Haste & Resistance**
- **Stamina & Mana Regeneration**

### 🎯 Hostile Ranged Mob Nerfs
Certain ranged mob types in vanilla Cube World dealt disproportionately high burst damage. Their output is rebalanced:
- **Boomerang-wielding Hostiles**: Damage halved (`* 0.5f`).
- **Mage Hostiles**: Spell attack power halved (`* 0.5f`).

### 💰 Starter Gold Boost
Newly created characters receive a memory patch giving them **40 Gold** upon spawning to afford basic tools, food, or starting items smoothly.

---

## 3. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 4 1` | Enables Creature Updates. |
| `/cubeforge mod 4 0` | Disables Creature Updates. |
| `/mod 4 1` / `/mod 4 0` | Legacy alias for toggling Creature Updates. |

---

## 4. Technical Implementation

- **Class**: [`CreatureUpdatesMod`](file:///d:/Projects/CubeMegaMod/src/mods/creature_updates/CreatureUpdatesMod.h) (`src/mods/creature_updates/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Memory Patch**: Injects bytes at offset `0x83590` (`0xB8 0x28 0x00 0x00 0x00 0x90`) via `cubeforge::memory::MemoryHelper` during initialization to set starting gold to 40 (`0x28`).
- **Stat Interception Callbacks**: Overrides CWSDK calculation events:
  - `OnCreatureArmorCalculated`
  - `OnCreatureCriticalCalculated`
  - `OnCreatureAttackPowerCalculated`
  - `OnCreatureSpellPowerCalculated`
  - `OnCreatureHasteCalculated`
  - `OnCreatureHPCalculated`
  - `OnCreatureResistanceCalculated`
  - `OnCreatureRegenerationCalculated`
  - `OnCreatureManaGenerationCalculated`

