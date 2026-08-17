# Feature: Player Updates & Monk Class (ID: 11)

The **Player Updates Mod** extends character customization and introduces an entirely new playable class—the **Monk**—complete with dual specializations, custom spells, passive mana mechanics, and expanded playable race options.

---

## 1. The Monk Class

The Monk is an agile martial artist and primal spellcaster who fights with fists and specialized elemental magic.

### Equipment Compatibility
- Can equip all standard armor types.
- Weapon restrictions: Fists only (all traditional weapons restricted).

---

## 2. Monk Specializations

### Spec 0: Chieftain (Primal Brawler)
- **Ultimate (Bulwark)**: 20-second cooldown. Instantly restores the player to full HP.
- **Shift Ability (Fire Trail)**: Consumes mana to leave behind a burning trail of fire dealing continuous damage scaling with attack power and combo counter.
- **Passive**: Continuous Passive Mana Gain.
- **Middle Mouse**: Fast evasive martial dash.

### Spec 1: Sorcerer (Primal Caster)
- **Ultimate (Bulwark)**: 30-second cooldown. Spawns a restorative healing puddle on the ground and grants +20 Combo.
- **Shift Ability (Lava Puddle)**: Costs 10 Mana. Spawns a damaging pool of molten lava and gains +1 Combo.
- **Passive**: Continuous Passive Mana Gain.
- **Middle Mouse (Poison Puddle)**: Costs 50 Stamina. Drops a toxic poison puddle on the ground and gains +5 Combo.

---

## 3. Playable Races & Custom Appearances

Player Updates adds visual options and presets for distinct fantasy races:
- **Jester**
- **Spectrino**
- **Raccoon**
- **Vampire**
- **Skeleton Knight**
- **Archdruid**
- **Half-demon**

---

## 4. Technical Implementation

- **Class**: `PlayerUpdatesMod` (`src/mods/PlayerUpdatesMod/`)
- **Class Framework**: `CharacterClass` (`src/mods/PlayerUpdatesMod/CharacterClass.h`)
- **Monk Implementation**: `MonkClass` (`src/mods/PlayerUpdatesMod/classes/MonkClass.cpp`)
- **Custom Spells**:
  - `FullHealAbility.h`
  - `HealingPuddle.h`
  - `LavaPuddle.h`
  - `PoisonPuddle.h`
- **Memory Hooks**:
  - `CharacterCreationHandler.h` (class list expansion)
  - `OnGetUltimateAbilityID.h` & `OnGetUltimateAbilityCooldown.h`
  - `OnCreatureCanEquipItem.h` (weapon restrictions)
  - `OnGetShiftAbilityID.h` & `OnExecuteAbility.h`
  - `OnCreaturePassiveMana.h` (passive mana regeneration)
  - `TreasureFixHandler.h` (class-specific loot drops)
  - `OnGenerateCreatureAppearance.h` (custom race model generation)
