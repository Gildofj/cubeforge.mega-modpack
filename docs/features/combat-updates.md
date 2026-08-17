# Feature: Combat Updates (ID: 3)

The **Combat Updates Mod** enhances action-oriented combat mechanics by adding custom resource conversion skills, emergency combo-based healing, and directional dash / dodge maneuvers.

---

## 1. Hotkey Combat Abilities

| Key Binding | Ability Name | Resource Cost | Effect |
| :---: | :--- | :--- | :--- |
| **`1`** | **Mana to Stamina Conversion** | Variable Mana | Converts available mana directly into stamina points via `ConvertMTSAbility`. |
| **`2`** | **Full Heal (Combo Burst)** | 50 Combo Hits | Instantly restores the player character back to 100% maximum HP via `HealAbility`. |

---

## 2. Directional Dash & Movement Abilities

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
  /enable doubletap
  ```
- **Control Modifier Mode**: Hold `Left Control` + press `W`, `A`, `S`, or `D`.
  ```text
  /disable doubletap
  ```

---

## 3. Technical Implementation

- **Class**: `CombatUpdateMod` (`src/mods/CombatUpdatesMod/`)
- **DirectInput Polling**: `OnGetKeyboardState(BYTE* diKeys)` polls raw keyboard states via `cube::DButton` state machines (`Pressed`, `Held`, `DoubleTap`).
- **Ability Objects**:
  - `cube::ConvertMTSAbility` (`src/cwsdk-extension/ability/ConvertMTSAbility.h`)
  - `cube::HealAbility` (`src/cwsdk-extension/ability/HealAbility.h`)
  - `cube::FarJumpAbility` (`src/cwsdk-extension/ability/FarJumpAbility.h`)
