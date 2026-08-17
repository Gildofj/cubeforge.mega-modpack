# Feature: Beginner Mode (ID: 7)

The **Beginner Mode Mod** provides a smoother difficulty curve for early-game progression by scaling down hostile creature stats during the player's initial 5 levels.

---

## 1. Enemy Stat Scaling Formula

Whenever the local player's level is between 0 and 4 (levels 1 through 5 in-game), all hostile creatures receive a progressive stat multiplier calculated as:

$$\text{Multiplier} = \frac{\text{Level} + 1}{6.0}$$

### Scaling Breakdown by Level

| Player Level | Stat Multiplier | Relative Enemy Strength |
| :---: | :---: | :---: |
| **Level 1** | $1 / 6 \approx 16.67\%$ | ~17% of base strength |
| **Level 2** | $2 / 6 \approx 33.33\%$ | ~33% of base strength |
| **Level 3** | $3 / 6 = 50.00\%$ | 50% of base strength |
| **Level 4** | $4 / 6 \approx 66.67\%$ | ~67% of base strength |
| **Level 5** | $5 / 6 \approx 83.33\%$ | ~83% of base strength |
| **Level 6+** | $100\%$ | Full Vanilla stats (no scaling) |

---

## 2. Affected Attributes
The multiplier is dynamically applied to all hostile mobs (`EntityBehaviour::Hostile`) for:
- **Maximum Health (`HP`)**
- **Armor**
- **Physical Attack Power**
- **Spell Power**
- **Damage Resistance**

---

## 3. Technical Implementation

- **Class**: `BeginnerModeMod` (`src/mods/BeginnerModeMod/`)
- **Callbacks**: Hooks into CWSDK creature calculation callbacks:
  ```cpp
  static void ApplyBeginnerBuffs(cube::Creature* creature, float* stat)
  {
      if (creature->entity_data.hostility_type != (char)cube::Enums::EntityBehaviour::Hostile) return;
      int level = cube::GetGame()->GetPlayer()->entity_data.level + 1;
      if (level > 5) return;
      *stat *= (level / 6.f);
  }
  ```
