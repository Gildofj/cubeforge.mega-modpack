# In-Game Commands Reference

CubeMegaMod provides interactive in-game chat commands allowing players to toggle modules on the fly, switch controls, and execute debug operations without restarting Cube World.

---

## 1. Mod Management Commands

### Toggle Sub-Modules
Enable or disable specific features by their module ID. Settings are automatically saved to disk and remembered on your next play session.

```text
/mod <ID> <state>
```

- **`<ID>`**: The numeric identifier of the module (1 through 12).
- **`<state>`**: `1` to Enable, `0` to Disable.

#### Examples
- Enable Beginner Mode:
  ```text
  /mod 7 1
  ```
- Disable Sea Exploration:
  ```text
  /mod 1 0
  ```

#### Module ID Quick Table

| ID | Feature Name | Description |
| :---: | :--- | :--- |
| `1` | Sea Exploration | Underwater chests, oxygen mechanics, underwater bosses |
| `2` | Lore Interactions | Rewards from discovering lore objects |
| `3` | Combat Updates | Key 1 & 2 skills, double-tap / Ctrl movement abilities |
| `4` | Creature Updates | 50% pet buffs, 50% mage/boomerang nerfs, starter 40g |
| `5` | Shop Updates | Gem Trader & Item Vendor inventory overhauls, Spirit Cubes |
| `6` | World Generation | Biome islands via Simplex noise, any starter biome |
| `7` | Beginner Mode | Enemy stat reduction for player levels 1–5 |
| `8` | Region Lock Update | Gradual gear power decay based on distance |
| `9` | Weapon Upgrading | Smithy adaptation & weapon upgrade mechanics |
| `10` | Quest System | Procedural kill quests from NPCs |
| `11` | Player Updates | Monk class, specializations, custom race visuals |
| `12` | Stack Updates | Increased maximum inventory stack size (100) |

---

## 2. Combat & Control Configuration

### Double-Tap Control Toggle
Configure whether combat movement abilities (dashes) are activated by **Double-Tapping** WASD or by holding **Left Control + WASD**.

- **Enable Double-Tap**:
  ```text
  /enable doubletap
  ```
- **Disable Double-Tap** (switches trigger to `Left Control + WASD`):
  ```text
  /disable doubletap
  ```

---

## 3. Debug & Developer Commands

> [!NOTE]
> These commands are intended for testing, mod development, and inspecting animations.

### Spawn Custom Class NPC
Spawns an NPC with a specific class ID and gem trader appearance flags near the player:
```text
/class <class_id>
```
- **`<class_id>`**: Numeric ID of the class (e.g. `5` for custom classes).

### Trigger Animation State
Forces the local player character to enter a specific animation state ID:
```text
/t <animation_id>
```
- **`<animation_id>`**: Numeric index of the target animation state.
