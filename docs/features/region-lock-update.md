# Feature: Region Lock Update (ID: 8)

The **Region Lock Update Mod** rebalances Cube World's controversial region-lock mechanic, replacing abrupt stat loss when crossing borders with a smooth, distance-based gear effectiveness decay.

---

## 1. Mechanics & Distance Formula

In vanilla Cube World, equipment becomes instantly useless when entering adjacent regions unless it is marked as a plus (`+`) item.

CubeMegaMod replaces this with a gradual power fade formula:

$$\text{Effective Rarity} = \text{Base Rarity} - ((2 - \text{PlusFlag}) \times \text{Distance})$$

- **$\text{Base Rarity}$**: Star rating of the item ($0 = \text{1-Star}, 1 = \text{2-Star}, \dots$).
- **$\text{Distance}$**: Number of zone boundaries between current region and the item's origin region.
- **$\text{PlusFlag}$**: `1` if item is a `+` weapon/armor, `0` otherwise.

### Rarity Degradation Comparison

| Distance from Origin | Standard Gear Penalty | Plus (`+`) Gear Penalty |
| :---: | :---: | :---: |
| **Origin Region (0)** | Full Power (No penalty) | Full Power (No penalty) |
| **1 Region Away** | $-2$ Stars | $-1$ Star |
| **2 Regions Away** | $-4$ Stars | $-2$ Stars |
| **3 Regions Away** | $-6$ Stars (reaches 0) | $-3$ Stars |

*(If effective rarity drops below 0, it behaves as a basic tier item with value `-1` rather than breaking entirely).*

---

## 2. Technical Implementation

- **Class**: `RegionLockUpdateMod` (`src/mods/RegionLockUpdateMod/`)
- **Naked ASM Detour**: `ASMEffectiveItemRarity()` in `src/hooks/RegionLockUpdate.h` replaces the vanilla rarity check instruction at offset `0x10976D`.
- **Branch Patches**: Modifies conditional jumps at `0x10974B` and `0x109733` to force distance checks across all zones.
