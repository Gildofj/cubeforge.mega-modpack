# Feature: Weapon Upgrading & Adaptation (ID: 9)

The **Weapon Upgrade Mod** revives the beloved alpha weapon adaptation and smithing mechanics, allowing players to upgrade and convert their weapons at town Smithy NPCs.

---

## 1. Upgrade Mechanics & Rules

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

## 2. Technical Implementation

- **Class**: `WeaponUpgradeMod` (`src/mods/WeaponUpgradeMod/`)
- **Smithy Interaction Hook**: `OnCreatureTalk(cube::Game* game, cube::Creature* creature)` catches talks with `ClassType::Smithy`, brings up `game->gui.adaption_widget`, and triggers the inventory dialog at offset `0x102DB0`.
- **UI Button Patches**: Patches hover selection coordinates at offsets `0x2685D7` and `0x268754` with `0xF3 0x41 0x0F 0x10 0xF2 0x90` to ensure smooth GUI navigation.
- **Detour**: `ModifyWeaponUpgrade.h` handles runtime item transformation upon completing the forge interaction.
