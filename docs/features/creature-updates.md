# Feature: Creature Updates (ID: 4)

The **Creature Updates Mod** balances creature stats, enhances the viability of companion pets, adjusts the difficulty of overtuned ranged mobs, and provides a smoother economic start for newly created characters.

---

## 1. Overview & Stat Adjustments

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

## 2. Technical Implementation

- **Class**: `CreatureUpdatesMod` (`src/mods/CreatureUpdatesMod/`)
- **Memory Patch**: Injects bytes at offset `0x83590` (`0xB8 0x28 0x00 0x00 0x00 0x90`) during initialization to set starting gold to 40 (`0x28`).
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
