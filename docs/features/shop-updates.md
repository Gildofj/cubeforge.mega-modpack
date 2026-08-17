# Feature: Shop Updates (ID: 5)

The **Shop Updates Mod** enriches town economies by adding rare commodities, artifact purchases, movement utility items, and classic Spirit Cubes to vendor inventories.

---

## 1. Vendor Inventories

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

## 2. Dynamic Price Handler

Item buy/sell prices are dynamically patched to ensure fair economies:
- Custom spirit cubes and special items resolve through `SetupItemPriceHandler` (`src/hooks/ItemPriceHandler.h`).

---

## 3. Technical Implementation

- **Class**: `ShopUpdateMod` (`src/mods/ShopUpdateMod/`)
- **Hook**: `SetupShopInteractionHandler` (`src/hooks/ShopInteractionHandler.h`) intercepts shop window generation at offset `0x94635` / `0x959E0`.
- **Item Injections**: Modifies the `itemVector` in `OnShopInteraction` and tracks sold items via `cube::Helper::CWGetItemsSold()` to prevent infinite purchase exploits.
