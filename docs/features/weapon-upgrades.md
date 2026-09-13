# Feature: Weapon Upgrading & Adaptation (ID: 9)

The **Weapon Upgrades** sub-mod (`cubeforge-weapon-upgrades.dll`) revives the beloved alpha weapon adaptation and smithing mechanics, allowing players to upgrade and convert their weapons at town Smithy NPCs.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-weapon-upgrades.dll` (`src/mods/weapon_upgrades/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-weapon-upgrades.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Upgrade Mechanics & Rules

NPCs tagged as **Smithy** appear in towns near smelting forges (where iron ore is melted). Interacting with them opens the adaptation widget interface.

### Upgrade Pathways
1. **Standard Weapon $\rightarrow$ Plus (`+`) Weapon**:
   - Upgrades a standard regional weapon into a `+` weapon.
   - Increases regional distance tolerance (reduces region lock penalty).
2. **Plus (`+`) Weapon $\rightarrow$ Higher Rarity Weapon**:
   - Upgrades a `+` weapon into a standard weapon with **+1 Extra Star of Rarity**.
3. **Spirit Cube Destruction**:
   - Upgrading a weapon consumes/destroys any spirit cubes currently attached to that weapon!

---

## 3. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 9 1` | Enables Weapon Upgrades & Smithy Adaptation. |
| `/cubeforge mod 9 0` | Disables Weapon Upgrades. |
| `/mod 9 1` / `/mod 9 0` | Legacy alias for toggling Weapon Upgrades. |

---

## 4. Technical Implementation

- **Class**: [`WeaponUpgradeMod`](file:///d:/Projects/CubeMegaMod/src/mods/weapon_upgrades/WeaponUpgradeMod.h) (`src/mods/weapon_upgrades/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Smithy Interaction**: `OnCreatureTalk(cube::Game* game, cube::Creature* creature)` catches talks with `ClassType::Smithy`, brings up `game->gui.adaption_widget`, and triggers the inventory dialog at offset `0x102DB0`.
- **UI Button Patches**: Patches hover selection coordinates at offsets `0x2685D7` and `0x268754` with `0xF3 0x41 0x0F 0x10 0xF2 0x90` via `cubeforge::memory::MemoryHelper` to ensure smooth GUI navigation.
- **Native MASM Detours** (`src/mods/weapon_upgrades/asm/hooks_weapon_upgrades.asm`): Handles runtime item transformation upon completing the forge interaction.

