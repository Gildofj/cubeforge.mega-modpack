# CubeMegaMod

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-blue.svg)](https://microsoft.com)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C.svg)](https://isocpp.org)
[![Build System](https://img.shields.io/badge/Build-CMake%20%7C%20Clang-green.svg)](https://cmake.org)
[![Target](https://img.shields.io/badge/Target-Cube%20World%20Steam-ff69b4.svg)](https://store.steampowered.com/app/1128000/Cube_World/)

**CubeMegaMod** is an expansive, modular gameplay overhaul for the Steam release of **Cube World**. Built as a native x64 dynamic link library (`DLL`) leveraging the **Cube World SDK (CWSDK)**, it revitalizes exploration, combat fluidity, regional progression, and town economies while preserving seamless multiplayer compatibility.

> [!IMPORTANT]
> **Backup your save data before playing with mods!**
> Save files are stored in `%LOCALAPPDATA%\CubeWorld\Save\` or `<CubeWorldFolder>\Save\`.

---

## 📑 Table of Contents
- [✨ Key Features](#-key-features)
- [📦 Quick Installation](#-quick-installation)
- [🎮 In-Game Commands](#-in-game-commands)
- [🏛️ Architecture & Modularity](#-architecture--modularity)
- [📚 Documentation Index](#-documentation-index)
- [🛠️ Building from Source](#-building-from-source)
- [🤝 Contributing](#-contributing)
- [❤️ Credits & Acknowledgements](#-credits--acknowledgements)

---

## ✨ Key Features

CubeMegaMod is split into **12 independently toggleable sub-modules**. Every feature can be enabled or disabled on the fly in-game via chat commands without restarting your client.

| ID | Feature Module | Summary | Documentation |
| :---: | :--- | :--- | :---: |
| **1** | **Sea Exploration** | Diving oxygen economy (10g / 10s), 4 underwater chest tiers (Wood, Skull, Obsidian, Bone), and deep-sea bosses. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/sea-exploration.md) |
| **2** | **Lore Interactions** | Lore inspection rolls for progressive loot (1–3 star gear, rare artifacts) with rich atmospheric flavor text. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/lore-interaction.md) |
| **3** | **Combat Updates** | Mana-to-Stamina conversion (Key 1), 50-combo Full Heal (Key 2), and WASD directional dashes (Double-Tap or Ctrl). | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/combat-updates.md) |
| **4** | **Creature Updates** | +50% all companion pet stats, 50% damage nerf to overtuned hostile boomerang & mage mobs, starter 40 Gold. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/creature-updates.md) |
| **5** | **Shop Updates** | Gem Trader sells artifacts, potions, and spirit cubes; Item Vendor stocks traversal gear, void stones, and rotating daily spirit cubes. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/shop-updates.md) |
| **6** | **World Generation** | Un-restricts starting spawn biomes and generates macro-islands with coherent biomes using Simplex Noise. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/world-generation.md) |
| **7** | **Beginner Mode** | Smooths early-game difficulty by scaling down hostile creature stats during player levels 1–5. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/beginner-mode.md) |
| **8** | **Region Lock Update** | Replaces abrupt border penalties with distance-based gear power decay ($-2$ stars/region for normal gear, $-1$ for `+` gear). | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/region-lock-update.md) |
| **9** | **Weapon Upgrades** | Revives alpha-style Smithy weapon adaptation: upgrade normal weapons to `+`, and `+` weapons to $+1$ star rarity. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/weapon-upgrades.md) |
| **10** | **Quest System** | Procedural kill quests dropped by NPCs targeting 30+ mob types with automatic inventory tracking and payouts. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/quest-system.md) |
| **11** | **Player Updates** | Introduces the **Monk** class (Chieftain & Sorcerer specs, custom spells, passive mana) and 7 new custom playable races. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/player-updates.md) |
| **12** | **Stack Updates** | Quality of life binary patch increasing maximum inventory stack size to **100** for all items and materials. | [Read Docs](file:///d:/Projects/CubeMegaMod/docs/features/stack-updates.md) |

---

## 📦 Quick Installation

1. Install the **[Cube World Mod Launcher](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher/releases)** by extracting it to your Cube World directory.
2. Download the latest `CubeMegaMod-vX.X.X.dll` from the [Releases page](https://github.com/Nichiren/CubeMegaMod/releases).
3. Place `CubeMegaMod-vX.X.X.dll` into your `<CubeWorldFolder>/Mods/` directory.
4. Launch the game via the Mod Launcher.

For detailed step-by-step instructions and troubleshooting, see the [Installation Guide](file:///d:/Projects/CubeMegaMod/docs/guides/INSTALLATION.md).

---

## 🎮 In-Game Commands

Open the in-game chat (`Enter`) to run commands:

```text
/mod <ID> <1/0>          - Enable (1) or Disable (0) a sub-module by its ID
/enable doubletap        - Enable double-tapping WASD to perform combat dashes
/disable doubletap       - Switch dash trigger to Left Control + WASD
/class <class_id>        - Spawn an NPC with a specific class for testing
/t <anim_id>             - Trigger a specific player animation state
```

For the complete command syntax and parameter details, see [Commands Reference](file:///d:/Projects/CubeMegaMod/docs/COMMANDS.md).

---

## 🏛️ Architecture & Modularity

CubeMegaMod is designed around a modular object-oriented architecture:
- **`CubeMod` Base Class**: Common interface for sub-mods with lifecycle hooks (`OnGameTick`, `OnChat`, `OnChestInteraction`, `OnLoreIncrease`, `OnCreatureDeath`, etc.).
- **Memory & Detour Engine**: Utilizes naked assembly stubs (`__attribute__((naked))`) and far jumps (`WriteFarJMP`) to hook game routines with minimal overhead.
- **CWSDK Extension Layer**: High-level C++ wrappers for abilities, UI inputs (`DButton`), dynamic quests, and entity manipulation.

For deep technical insights, see [Architecture & Design](file:///d:/Projects/CubeMegaMod/docs/ARCHITECTURE.md).

---

## 📚 Documentation Index

All project documentation is organized under [`docs/`](file:///d:/Projects/CubeMegaMod/docs/):

- **System Architecture**: [docs/ARCHITECTURE.md](file:///d:/Projects/CubeMegaMod/docs/ARCHITECTURE.md)
- **Commands Reference**: [docs/COMMANDS.md](file:///d:/Projects/CubeMegaMod/docs/COMMANDS.md)
- **Contributing Guidelines**: [docs/CONTRIBUTING.md](file:///d:/Projects/CubeMegaMod/docs/CONTRIBUTING.md)
- **Guides**:
  - [Player Installation Guide](file:///d:/Projects/CubeMegaMod/docs/guides/INSTALLATION.md)
  - [Building from Source (VS2019 / Clang / CMake)](file:///d:/Projects/CubeMegaMod/docs/guides/BUILDING.md)
  - [Creating a Sub-Mod Guide](file:///d:/Projects/CubeMegaMod/docs/guides/CREATING-A-MOD.md)
- **Feature Documentation**:
  - [Sea Exploration (ID: 1)](file:///d:/Projects/CubeMegaMod/docs/features/sea-exploration.md)
  - [Lore Interactions (ID: 2)](file:///d:/Projects/CubeMegaMod/docs/features/lore-interaction.md)
  - [Combat Updates (ID: 3)](file:///d:/Projects/CubeMegaMod/docs/features/combat-updates.md)
  - [Creature Updates (ID: 4)](file:///d:/Projects/CubeMegaMod/docs/features/creature-updates.md)
  - [Shop Updates (ID: 5)](file:///d:/Projects/CubeMegaMod/docs/features/shop-updates.md)
  - [World Generation (ID: 6)](file:///d:/Projects/CubeMegaMod/docs/features/world-generation.md)
  - [Beginner Mode (ID: 7)](file:///d:/Projects/CubeMegaMod/docs/features/beginner-mode.md)
  - [Region Lock Update (ID: 8)](file:///d:/Projects/CubeMegaMod/docs/features/region-lock-update.md)
  - [Weapon Upgrades (ID: 9)](file:///d:/Projects/CubeMegaMod/docs/features/weapon-upgrades.md)
  - [Quest System (ID: 10)](file:///d:/Projects/CubeMegaMod/docs/features/quest-system.md)
  - [Player Updates & Monk (ID: 11)](file:///d:/Projects/CubeMegaMod/docs/features/player-updates.md)
  - [Stack Updates (ID: 12)](file:///d:/Projects/CubeMegaMod/docs/features/stack-updates.md)

---

## 🛠️ Building from Source

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/Nichiren/CubeMegaMod.git
cd CubeMegaMod

# Open in Visual Studio 2019/2022 and build with x64-Clang-Release
```

Refer to the [Building Guide](file:///d:/Projects/CubeMegaMod/docs/guides/BUILDING.md) for full compilation steps.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Check out our [Contributing Guidelines](file:///d:/Projects/CubeMegaMod/docs/CONTRIBUTING.md) before opening a pull request.

---

## ❤️ Credits & Acknowledgements

- **ChrisMiuchiz**: For creating the **CWSDK** and **Cube World Mod Launcher**, which make modern Cube World modding possible.
- **TheBagel3**: For invaluable support, testing, video showcases, and community enthusiasm.
- **Timothy (GrayShadoz)**, **Raxiti**, **kuba.dlouhej**: For testing, reporting issues, and continuous feedback.
- **Andoryuuta & matpow2**: For reverse engineering foundation and struct mappings.

---

## 📄 License
This project is licensed under the [MIT License](file:///d:/Projects/CubeMegaMod/LICENSE).
