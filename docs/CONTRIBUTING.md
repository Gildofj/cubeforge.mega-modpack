# Contributing to CubeMegaMod

Thank you for your interest in contributing to **CubeMegaMod**! This document provides guidelines for contributing code, reporting bugs, and implementing new features.

---

## 1. Code of Conduct & Philosophy
- **Respectful & Collaborative**: Be constructive in code reviews and discussions.
- **Modularity First**: All new gameplay mechanics should be isolated inside their own sub-mod class inheriting from `CubeMod`.
- **Non-Destructive Hooks**: Avoid hard-overwriting unrelated game functions whenever possible. Use focused memory patches and restore register states cleanly.

---

## 2. Development Environment Setup

### Prerequisites
1. **Windows 10 / 11 (x64)**
2. **Visual Studio 2019** (Community edition is sufficient)
3. **Clang / LLVM toolset for Visual Studio** (`C++ Clang Compiler for Windows`)
4. **CMake 3.8+**
5. **Cube World (Steam Beta Version)**
6. **[Cube World Mod Launcher](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher)**

### Clone the Repository
```bash
git clone --recurse-submodules https://github.com/Nichiren/CubeMegaMod.git
cd CubeMegaMod
```

For full compilation details, see the [Building Guide](file:///d:/Projects/CubeMegaMod/docs/guides/BUILDING.md).

---

## 3. Pull Request Guidelines

1. **Branch Naming**:
   - `feature/feature-name` for new mechanics or classes.
   - `fix/bug-description` for bug fixes.
   - `docs/doc-update` for documentation changes.
2. **Commit Style**:
   - Use clear, descriptive commit messages:
     ```text
     feat(player-updates): add custom Monk starter gear
     fix(region-lock): prevent negative rarity calculations
     docs: add comprehensive docs directory
     ```
3. **Code Formatting**:
   - Follow standard C++17 conventions.
   - Indent using tabs (or 4 spaces consistently).
   - Keep comments focused on **why** a detour/patch is performed rather than stating trivial ASM instructions.

---

## 4. Testing Your Changes

Before submitting a Pull Request:
1. Build the mod in `x64-Clang-Release` mode.
2. Copy the resulting `.dll` into your Cube World `Mods/` directory.
3. Launch Cube World via the Mod Launcher.
4. Verify that:
   - The mod initialization popup appears without crashes.
   - Chat commands (`/mod <ID> <0/1>`) toggle the feature cleanly.
   - Save files in `Save/` write and load without corruption.
   - Multiplayer / single-player zone transitions operate normally.

---

## 5. Adding New Sub-Mods or Classes
Refer to our dedicated guide: [Creating a Mod](file:///d:/Projects/CubeMegaMod/docs/guides/CREATING-A-MOD.md) for a step-by-step tutorial on registering a new `CubeMod` class and hooking game callbacks.
