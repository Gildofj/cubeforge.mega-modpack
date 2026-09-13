# In-Game Commands Reference: cubeforge.mega-modpack

**cubeforge.mega-modpack** provides interactive in-game chat commands allowing players to monitor status, toggle modules on the fly, customize controls, and execute debug operations without restarting Cube World.

---

## 1. Unified CubeForge Commands

Open the in-game chat (`Enter`) to execute commands:

| Command | Description |
| :--- | :--- |
| `/cubeforge status` (or `/cubeforge`) | Displays active/inactive mod counts and status summary. |
| `/cubeforge list` | Lists all 12 modules, their IDs, versions, and current states. |
| `/cubeforge help` | Prints the full commands help manual in chat. |
| `/cubeforge mod <id> <1/0>` | Toggles a specific module on (`1`) or off (`0`). |
| `/cubeforge combat doubletap <1/0>` | Enables/disables WASD double-tap dash trigger. |
| `/cubeforge sea autogold <1/0>` | Enables/disables automatic gold deduction for diving oxygen. |
| `/cubeforge class <id>` | Spawns a test NPC with a specific class ID. |
| `/cubeforge anim <id>` | Plays a specific animation state on the local player. |

---

## 2. Legacy Aliases (Backwards Compatibility)

For players accustomed to legacy CubeMegaMod syntax, all original shortcuts remain functional:

- **Module Toggle**:
  ```text
  /mod <id> <1/0>
  ```
- **Double-Tap Dodge**:
  ```text
  /enable doubletap
  /disable doubletap
  ```
- **Auto-Gold Diving**:
  ```text
  /enable autogoldusage
  /disable autogoldusage
  ```
- **Debug / Class / Animation**:
  ```text
  /class <class_id>
  /t <anim_id>
  ```

---

## 3. Module ID Quick Table

| ID | Feature Name | Description |
| :---: | :--- | :--- |
| `1` | Sea Exploration | Underwater chests, oxygen mechanics, deep-sea bosses |
| `2` | Lore Interactions | Rewards from discovering lore objects |
| `3` | Combat Updates | Key 1 & 2 skills, double-tap / Ctrl movement abilities |
| `4` | Creature Updates | 50% pet buffs, 50% mage/boomerang nerfs, starter 40g |
| `5` | Shop Updates | Gem Trader & Item Vendor inventory overhauls, Spirit Cubes |
| `6` | World Generation | Biome islands via Simplex noise, any starter biome |
| `7` | Beginner Mode | Enemy stat reduction for player levels 1–5 |
| `8` | Region Lock Update | Gradual gear power decay based on distance |
| `9` | Weapon Upgrades | Smithy adaptation & weapon upgrade mechanics |
| `10` | Quest System | Procedural kill quests from NPCs |
| `11` | Player Updates | Monk class, specializations, custom race visuals |
| `12` | Stack Updates | Increased maximum inventory stack size (100) |
