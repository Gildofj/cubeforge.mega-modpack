# Feature: Player Updates & Monk Class (ID: 11)

The **Player Updates** sub-mod (`cubeforge-player-updates.dll`) extends character customization and introduces an entirely new playable class—the **Monk**—complete with dual specializations, custom elemental spells, passive mana mechanics, and expanded playable race options.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-player-updates.dll` (`src/mods/player_updates/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-player-updates.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. The Monk Class

The Monk is an agile martial artist and primal spellcaster who fights with fists and specialized elemental magic.

### Equipment Compatibility
- Can equip all standard armor types.
- Weapon restrictions: Fists only (all traditional weapons restricted).

---

## 3. Monk Specializations

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

## 4. Playable Races & Custom Appearances

Player Updates adds visual options and presets for distinct fantasy races:
- **Jester**
- **Spectrino**
- **Raccoon**
- **Vampire**
- **Skeleton Knight**
- **Archdruid**
- **Half-demon**

---

## 5. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 11 1` | Enables Player Updates & Monk Class. |
| `/cubeforge mod 11 0` | Disables Player Updates. |
| `/cubeforge class <id>` | Spawns a test NPC creature with class ID. |
| `/cubeforge anim <id>` | Plays an animation state on the local player. |
| `/mod 11 1` / `/mod 11 0` | Legacy alias for toggling Player Updates. |

---

## 6. Technical Implementation

- **Class**: [`PlayerUpdatesMod`](file:///d:/Projects/CubeMegaMod/src/mods/player_updates/PlayerUpdatesMod.h) (`src/mods/player_updates/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Class Framework**: `CharacterClass` & `MonkClass` (`src/mods/player_updates/classes/`)
- **Custom Spells**:
  - `FullHealAbility.h`
  - `HealingPuddle.h`
  - `LavaPuddle.h`
  - `PoisonPuddle.h`
- **Native MASM Detours & Memory Hooks** (`src/mods/player_updates/asm/hooks_player_updates.asm`):
  - Character creation menu class list expansion
  - Ultimate ability ID and cooldown overrides
  - Equipment restriction checks
  - Shift ability activation and execution
  - Passive mana regeneration ticks
  - Treasure drop class adaptation
  - Procedural race appearance generator

