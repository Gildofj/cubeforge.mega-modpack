# Feature: Lore Interactions (ID: 2)

The **Lore Interactions** sub-mod (`cubeforge-lore-interactions.dll`) makes lore discovery rewarding by dropping scaled equipment and rare artifacts whenever the player inspects and increases regional lore, accompanied by randomized atmospheric chat descriptions.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-lore-interactions.dll` (`src/mods/lore_interactions/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-lore-interactions.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Reward Scaling Tiers

When inspecting lore objects (e.g. ancient ruins, inscriptions), a random roll `[0..99]` determines whether an item drops based on the player's current lore percentage:

| Current Lore Value | Drop Calculation | Reward Dropped |
| :--- | :--- | :--- |
| **Lore < 100%** | Chance = `min(Lore, 50%)` | 1-Star Regional Gear item (`rarity = 0`) |
| **100% ≤ Lore < 150%** | Chance = `(Lore - 100)%` | 2-Star to 3-Star Regional Weapon (`rarity = 1..2`) |
| **Lore ≥ 150%** | Chance = `min((Lore - 150) / 2, 40%)` | Rare Artifact item |

---

## 3. Dynamic Discovery Flavor Messages

Whenever an item drops from a lore interaction, the mod formats a rich atmospheric sentence printed in orange chat text (`RGB: 255, 165, 0`):

> *"You [found / stumbled upon / uncovered] [some gear / a weapon / an artifact] [hidden behind the description / buried in the ground / covered in mud / next to the object]!"*

---

## 4. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 2 1` | Enables Lore Interactions. |
| `/cubeforge mod 2 0` | Disables Lore Interactions. |
| `/mod 2 1` / `/mod 2 0` | Legacy alias for toggling Lore Interactions. |

---

## 5. Technical Implementation

- **Class**: [`LoreInteractionMod`](file:///d:/Projects/CubeMegaMod/src/mods/lore_interactions/LoreInteractionMod.h) (`src/mods/lore_interactions/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Assembly Hook**: Hooks lore counter increments at base offset `0x103A89` via native MASM detour (`asm/hooks_lore_interactions.asm`).
- **Dispatch**: `LoreInteractionMod::OnLoreIncrease(cube::Game* game, int value)` processes the chance calculation and calls `cube::Helper::DropItem()`.

