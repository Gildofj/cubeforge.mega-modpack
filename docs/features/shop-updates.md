# Feature: Shop Updates (ID: 5)

The **Shop Updates** sub-mod (`cubeforge-shop-updates.dll`) enriches town economies by adding rare commodities, artifact purchases, movement utility items, and classic Spirit Cubes to vendor inventories.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-shop-updates.dll` (`src/mods/shop_updates/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-shop-updates.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Vendor Inventories

### 💎 Gem Trader Shop
The Gem Trader vendor has been restructured to offer high-value progression items:

| Item | Cost | Quantity / Rules |
| :--- | :---: | :--- |
| **Random Artifact** | ~1,000g | 1 unique artifact seeded per vendor ID. |
| **Healing Potions (Tiers 1–4)** | 5g – 20g | Up to 3 of each tier (useful for non-healing classes in hostile biomes). |
| **Spirit Cubes (Random Element)** | 25g | 1 to 3 cubes (50% random daily chance; Fire, Ice, Wind, Unholy). |

*(Vanilla basic gem listings are removed to make room for useful items).*

### 🎒 Item Vendor Shop
The Item Vendor provides specialized exploration and upgrading equipment:

| Item | Cost | Quantity / Rules |
| :--- | :---: | :--- |
| **Special Movement Items** | 75g | Gliders, boats, reins, climbing spikes, etc. |
| **Void Stone** | 50g | 1 per vendor. |
| **Daily Spirit Cubes** | 25g | 1 to 3 random Spirit Cubes whose stock and element rotate daily based on world state day. |

---

## 3. Dynamic Price Handler

Item buy/sell prices are dynamically handled to ensure fair economies:
- Custom spirit cubes and special items resolve through price hooks that accurately compute vendor values and sellback rates.

---

## 4. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 5 1` | Enables Shop Updates. |
| `/cubeforge mod 5 0` | Disables Shop Updates. |
| `/mod 5 1` / `/mod 5 0` | Legacy alias for toggling Shop Updates. |

---

## 5. Technical Implementation

- **Class**: [`ShopUpdateMod`](file:///d:/Projects/CubeMegaMod/src/mods/shop_updates/ShopUpdateMod.h) (`src/mods/shop_updates/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Native MASM Detours & Hooks** (`src/mods/shop_updates/asm/hooks_shop_updates.asm`):
  - Intercepts shop window generation at offset `0x94635` / `0x959E0`
  - Item buy/sell price evaluation hooks
- **Item Injections**: Modifies the `itemVector` in `OnShopInteraction` and tracks sold items via `cube::Helper::CWGetItemsSold()` to prevent infinite purchase exploits.

