# Feature: Stack Updates (ID: 12)

The **Stack Updates** sub-mod (`cubeforge-stack-updates.dll`) provides a vital quality-of-life improvement by increasing item stack limits across the inventory from default low thresholds (e.g. 50) up to **100**.

---

## 1. Target & Deployment

- **Standalone Target**: `cubeforge-stack-updates.dll` (`src/mods/stack_updates/`)
- **ModPack Bundle**: Integrated into `cubeforge-megamod.dll` (`src/modpack/`)
- **Persistence File**: `Mods/cubeforge-stack-updates.sav` (or `Mods/cubeforge-megamod.sav`)

---

## 2. Overview & Mechanics

- **Stack Capacity**: All stackable items—potions, ingredients, materials, spirit cubes, and consumables—can now stack up to **100 units per slot**.
- **Unified Sizing**: Replaces category-dependent stack size logic with a unified max capacity check.

---

## 3. In-Game Commands & Configuration

| Command | Action |
| :--- | :--- |
| `/cubeforge mod 12 1` | Enables expanded item stacking. |
| `/cubeforge mod 12 0` | Disables mod (restores vanilla 50-stack limit). |
| `/mod 12 1` / `/mod 12 0` | Legacy alias for toggling Stack Updates. |

---

## 4. Technical Implementation & Memory Patches

`StackUpdatesMod` patches two key branching instructions in `cubeworld.exe` during initialization:

```cpp
void StackUpdatesMod::Initialize()
{
    auto offset_p1 = 0x50755;	// Ensure all stack sizes are the same.
    auto offset_p2 = 0x50789;	// Increase all stack sizes.

    // 1. Force branch to always take the expanded stack size path
    cubeforge::memory::MemoryHelper::WriteByte(CWOffset(offset_p1), 0xEB);       // Changes 0x75 (jne/jnz) -> 0xEB (jmp)

    // 2. Increase the maximum stack size integer constant from 50 (0x32) to 100 (0x64)
    cubeforge::memory::MemoryHelper::WriteByte(CWOffset(offset_p2 + 3), 0x64);   // Patches immediate operand to 100
}
```

- **Class**: [`StackUpdatesMod`](file:///d:/Projects/CubeMegaMod/src/mods/stack_updates/StackUpdatesMod.h) (`src/mods/stack_updates/`)
- **Lifecycle Base**: [`BaseMod`](file:///d:/Projects/CubeMegaMod/src/core/BaseMod.h)
- **Binary Footprint**: 2 direct byte writes, zero CPU overhead per game tick.

