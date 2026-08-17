# Feature: Lore Interactions (ID: 2)

The **Lore Interactions Mod** makes lore discovery rewarding by dropping scaled equipment and rare artifacts whenever the player inspects and increases regional lore, accompanied by randomized atmospheric chat descriptions.

---

## 1. Reward Scaling Tiers

When inspecting lore objects (e.g. ancient ruins, inscriptions), a random roll `[0..99]` determines whether an item drops based on the player's current lore percentage:

| Current Lore Value | Drop Calculation | Reward Dropped |
| :--- | :--- | :--- |
| **Lore < 100%** | Chance = `min(Lore, 50%)` | 1-Star Regional Gear item (`rarity = 0`) |
| **100% ≤ Lore < 150%** | Chance = `(Lore - 100)%` | 2-Star to 3-Star Regional Weapon (`rarity = 1..2`) |
| **Lore ≥ 150%** | Chance = `min((Lore - 150) / 2, 40%)` | Rare Artifact item |

---

## 2. Dynamic Discovery Flavor Messages

Whenever an item drops from a lore interaction, the mod formats a rich atmospheric sentence printed in orange chat text (`RGB: 255, 165, 0`):

> *"You [found / stumbled upon / uncovered] [some gear / a weapon / an artifact] [hidden behind the description / buried in the ground / covered in mud / next to the object]!"*

---

## 3. Technical Implementation

- **Class**: `LoreInteractionMod` (`src/mods/LoreInteractionMod/`)
- **Hook**: `lore_increase.h` (`src/hooks/lore_increase.h`) intercepts lore counter increments at base offset `0x103A89` and queues a `HookEvent::LoreInteraction` event.
- **Dispatch**: `LoreInteractionMod::OnLoreIncrease(cube::Game* game, int value)` processes the chance calculation and calls `cube::Helper::DropItem()`.
