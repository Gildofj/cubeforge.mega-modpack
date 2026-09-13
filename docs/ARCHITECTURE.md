# Architecture & Technical Design: cubeforge.mega-modpack

**cubeforge.mega-modpack** is a modular C++ mod ecosystem for the Steam release of **Cube World**, built natively on top of the **CubeForge SDK (`cubeforge.sdk`)** and fully compatible with **`cubeforge.loader`**. It can be deployed as 12 individual standalone DLLs (`cubeforge-<nome>.dll`) or as a unified modpack (`cubeforge-megamod.dll`).

---

## 1. System Overview

```mermaid
flowchart TD
    CW[Cube World Steam Client] -->|Loads DLL| Loader[cubeforge.loader / ModLoader]
    Loader -->|Calls MakeMod| TargetDLL[cubeforge-*.dll / cubeforge-megamod.dll]
    TargetDLL --> BaseFramework[cubeforge-core Framework]
    
    subgraph Core Framework (cubeforge-core)
        BaseFramework --> BaseMod[BaseMod Lifecycle & Persistence]
        BaseFramework --> CmdHandler[Unified /cubeforge Command Router]
        BaseFramework --> MemHelper[MemoryHelper Detours & FarJMP]
        BaseFramework --> SDKExt[Creature, Input, Inventory, Abilities]
    end
    
    subgraph Sub-Mods Layer
        TargetDLL --> SubMods[12 Modular Standalone Sub-Mods]
    end
    
    subgraph Hooking & Detour Layer
        MemHelper -->|MASM x64 Detours| CW
    end
```

---

## 2. Core Components

### 2.1 Mod Lifecycle & Dispatcher (`src/core/BaseMod.h` / `.cpp`)
- **`GenericMod` Interface**: Inherits from CWSDK's `GenericMod` and exports `MakeMod()`.
- **`BaseMod` Base Class**: All 12 modular sub-mods inherit from `BaseMod`. Each sub-mod provides:
  - Unique identifier (`m_ID`)
  - Semantic versioning (`m_Version`)
  - Enable/Disable toggle (`m_Enabled`)
  - Binary state persistence routines (`Save()`, `Load()`) writing to `Mods/<FileName>.sav`
  - Lifecycle and event callbacks (`OnGameTick`, `OnChat`, `OnChestInteraction`, `OnLoreIncrease`, `OnCreatureDeath`, `OnShopInteraction`, etc.)

### 2.2 Memory Patching & Assembly Detours (`src/core/memory/`, `src/mods/*/asm/`)
The system uses modular assembly detours and runtime memory patching:
1. **Direct Memory Modification (`MemoryHelper::PatchMemory`, `WriteByte`)**:
   - Modifies opcodes dynamically with memory protection handling via Windows `VirtualProtect`.
2. **Far Jumps (`MemoryHelper::WriteFarJMP`)**:
   - Hooks game function entries with 14-byte absolute jumps to custom MASM x64 routines.
3. **MASM x64 Detours**:
   - Preserves CPU register state with `PUSH_ALL` / `POP_ALL`.
   - Stack alignment using `PREPARE_STACK` and `RESTORE_STACK`.
   - Transfers control to C++ event handlers and returns to the game loop.

---

## 3. Core Framework Sub-systems (`src/core/`)

| Sub-system | Path | Description |
| :--- | :--- | :--- |
| **Abilities** | `src/core/abilities/` | Combat skills (`HealAbility`, `ConvertMTSAbility`, `FarJumpAbility`, `EventList`). |
| **Input** | `src/core/input/` | `DButton` class tracking DirectInput keyboard states (`Pressed`, `Held`, `DoubleTap`). |
| **Creatures** | `src/core/creature/` | `CreatureFactory` for runtime spawning, bosses, chests, and creatures. |
| **Inventory** | `src/core/inventory/` | Item manipulation, timer abstraction, and inventory structures. |
| **Memory** | `src/core/memory/` | `MemoryHelper` with `MemoryProtectGuard` RAII protection and FarJMP patching. |
| **Utils** | `src/core/utils/` | `GameHelper` world collision, drops, and RNG utilities. |

---

## 4. Module Registry & Sub-Mod Matrix

| ID | Module Name | Standalone DLL | Class Name | Responsibility |
| :---: | :--- | :--- | :--- | :--- |
| **1** | Sea Exploration | `cubeforge-sea-exploration.dll` | `SeaExplorationMod` | Diving oxygen stamina loop, custom underwater chests, deep-sea bosses. |
| **2** | Lore Interactions | `cubeforge-lore-interactions.dll` | `LoreInteractionMod` | Progressive reward drops & flavor text upon discovering lore. |
| **3** | Combat Updates | `cubeforge-combat-updates.dll` | `CombatUpdateMod` | Key-bindable combat abilities, mana/stamina converter, double-tap dashes. |
| **4** | Creature Updates | `cubeforge-creature-updates.dll` | `CreatureUpdatesMod` | Pet stat buff multipliers, hostile boomerang/mage damage nerfs, starter gold. |
| **5** | Shop Updates | `cubeforge-shop-updates.dll` | `ShopUpdateMod` | Gem Trader & Item Vendor inventory injections, Spirit Cubes, dynamic pricing. |
| **6** | World Generation | `cubeforge-world-gen.dll` | `WorldGenMod` | Non-restricted spawn biomes, Simplex noise macro-islands, building overrides. |
| **7** | Beginner Mode | `cubeforge-beginner-mode.dll` | `BeginnerModeMod` | Dynamic stat reduction for hostile mobs during player levels 1–5. |
| **8** | Region Lock Update | `cubeforge-region-lock.dll` | `RegionLockMod` | Softens region lock decay formula based on distance and `+` item modifier. |
| **9** | Weapon Upgrades | `cubeforge-weapon-upgrades.dll` | `WeaponUpgradeMod` | Revives alpha Smithy adaptation and item tier upgrade mechanics. |
| **10** | Quest System | `cubeforge-quest-system.dll` | `QuestMod` | Procedural kill quests from NPCs, kill tracking, and automated completion. |
| **11** | Player Updates | `cubeforge-player-updates.dll` | `PlayerUpdatesMod` | Custom class system (`MonkClass`), abilities, specs, custom race appearances. |
| **12** | Stack Updates | `cubeforge-stack-updates.dll` | `StackUpdatesMod` | Binary patch expanding item stack limit to 100 across all categories. |
| **—** | **MegaModPack Bundle** | **`cubeforge-megamod.dll`** | `MegaModPack` | **Aggregated bundle consolidating all 12 modules into a single binary.** |

---

## 5. State Persistence

Each sub-mod serializes its settings to the `Mods/` directory:
- Directory: `<CubeWorldFolder>/Mods/`
- Format: Binary struct serialization (`.sav`)
- Configuration toggle states (`/cubeforge mod <ID> <0/1>`) are saved automatically to `Mods/<FileName>.sav`.
