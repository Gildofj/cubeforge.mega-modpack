# Feature: Combat Updates (ID: 3)

The **Combat Updates** sub-mod (`cubeforge-combat-updates.dll`) enhances action-oriented combat mechanics by adding custom resource conversion skills, emergency combo-based healing, and directional dash / dodge maneuvers.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-combat-updates.dll` (`src/mods/combat_updates/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-combat-updates.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Hotkey Combat Abilities

| Key Binding | Ability Name | Resource Cost | Effect |
| :---: | :--- | :--- | :--- |
| **`1`** | **Mana to Stamina Conversion** | Variable Mana | Converts available mana directly into stamina points via `ConvertMTSAbility`. |
| **`2`** | **Full Heal (Combo Burst)** | 50 Combo Hits | Instantly restores the player character back to 100% maximum HP via `HealAbility`. |

---

## 3. Directional Dash & Movement Abilities

Players can execute quick dashes in all 4 cardinal movement directions:

- **Forward Dash (`W`)**: Propels player forward.
- **Left Dash (`A`)**: Sidesteps left.
- **Backward Evade (`S`)**: Quick backstep escape / attack stance.
- **Right Dash (`D`)**: Sidesteps right.
- **Stamina Cost**: 75 Stamina per activation.

### Activation Mode

The trigger method can be configured in chat:
- **Double-Tap Mode** (Default): Double-tap `W`, `A`, `S`, or `D`.
  ```text
  /cubeforge combat doubletap 1
  ```
- **Control Modifier Mode**: Hold `Left Control` + press `W`, `A`, `S`, or `D`.
  ```text
  /cubeforge combat doubletap 0
  ```

---

## 4. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 3 1` | Enables Combat Updates. |
| `/cubeforge mod 3 0` | Disables Combat Updates. |
| `/cubeforge combat doubletap 1` | Enables double-tap dash mode. |
| `/cubeforge combat doubletap 0` | Disables double-tap dash mode (requires Ctrl modifier). |
| `/enable doubletap` / `/disable doubletap` | Legacy aliases for double-tap toggling. |

---

## 5. Technical Implementation

- **Class**: [`CombatUpdateMod`](file:///d:/Projects/CubeMegaMod/src/mods/combat_updates/CombatUpdateMod.h) (`src/mods/combat_updates/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **DirectInput Polling**: `OnGetKeyboardState(BYTE* diKeys)` polls raw keyboard states via `cube::DButton` state machines (`Pressed`, `Held`, `DoubleTap`).
- **Core Ability System**:
  - `cube::ConvertMTSAbility` (`src/core/abilities/ConvertMTSAbility.h`)
  - `cube::HealAbility` (`src/core/abilities/HealAbility.h`)
  - `cube::FarJumpAbility` (`src/core/abilities/FarJumpAbility.h`)
  - `cube::EventList` (`src/core/abilities/EventList.h`)

