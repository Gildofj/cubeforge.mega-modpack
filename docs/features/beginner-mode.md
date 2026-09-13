# Feature: Beginner Mode (ID: 7)

The **Beginner Mode** sub-mod (`cubeforge-beginner-mode.dll`) provides a smooth, accessible difficulty curve for early-game progression by dynamically scaling down hostile creature stats while the local player is between levels 1 and 5.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-beginner-mode.dll` (`src/mods/beginner_mode/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-beginner-mode.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Enemy Stat Scaling Formula

Whenever the local player's level is between 0 and 4 (levels 1 through 5 in-game), all hostile creatures receive a progressive stat multiplier calculated as:

$$\text{Multiplier} = \frac{\text{Level} + 1}{6.0}$$

### Scaling Breakdown by Level

| Player Level | Level Index (`level`) | Stat Multiplier | Relative Enemy Strength |
| :---: | :---: | :---: | :---: |
| **Level 1** | 0 | $1 / 6 \approx 16.67\%$ | ~17% of base strength |
| **Level 2** | 1 | $2 / 6 \approx 33.33\%$ | ~33% of base strength |
| **Level 3** | 2 | $3 / 6 = 50.00\%$ | 50% of base strength |
| **Level 4** | 3 | $4 / 6 \approx 66.67\%$ | ~67% of base strength |
| **Level 5** | 4 | $5 / 6 \approx 83.33\%$ | ~83% of base strength |
| **Level 6+** | 5+ | $100.00\%$ | Full Vanilla stats (no scaling) |

---

## 3. Affected Attributes

The multiplier is dynamically applied to all hostile mobs (`EntityBehaviour::Hostile`) across the following core attributes:
- **Maximum Health (`HP`)**
- **Armor**
- **Physical Attack Power**
- **Spell Power**
- **Damage Resistance**

Friendly companion pets, neutral wildlife, and friendly NPCs are not affected by this reduction.

---

## 4. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 7 1` | Enables Beginner Mode scaling. |
| `/cubeforge mod 7 0` | Disables Beginner Mode scaling (vanilla difficulty). |
| `/mod 7 1` / `/mod 7 0` | Legacy alias for toggling Beginner Mode. |

---

## 5. Technical Implementation

- **Class**: [`BeginnerModeMod`](file:///d:/Projects/CubeMegaMod/src/mods/beginner_mode/BeginnerModeMod.h) (`src/mods/beginner_mode/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **CWSDK Calculation Callbacks**: Overrides creature stat calculation events:
  - `OnCreatureArmorCalculated`
  - `OnCreatureAttackPowerCalculated`
  - `OnCreatureSpellPowerCalculated`
  - `OnCreatureHPCalculated`
  - `OnCreatureResistanceCalculated`
