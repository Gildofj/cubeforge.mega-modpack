# Contributing to cubeforge.mega-modpack

Thank you for your interest in contributing to **cubeforge.mega-modpack**! This document provides guidelines for contributing code, reporting bugs, and implementing new features.

---

## 1. Code of Conduct & Philosophy
- **Respectful & Collaborative**: Be constructive in code reviews and discussions.
- **Modularity First**: All new gameplay mechanics should be isolated inside their own sub-mod target inheriting from `BaseMod`.
- **Non-Destructive Hooks**: Avoid hard-overwriting unrelated game functions whenever possible. Use focused memory patches and restore register states cleanly.
- **Strict Typing & Modern Standards**: C++20 standard, strict compilation, zero raw unmanaged leaks.

---

## 2. Development Environment Setup

### Prerequisites
1. **Windows 10 / 11 (x64)**
2. **Visual Studio 2022** (v143 toolset with MSVC C++ and MASM)
3. **CMake 3.25+**
4. **[cubeforge.sdk](https://github.com/Gildofj/cubeforge.sdk)**
5. **[cubeforge.loader](https://github.com/Gildofj/cubeforge.loader)** / Mod Launcher

### Clone the Repository
```bash
git clone https://github.com/Gildofj/cubeforge.mega-modpack.git
cd cubeforge.mega-modpack
```

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
   - Follow strict C++20 standards.
   - Self-documenting code over comments. Only comment "why," never "what."

---

## 4. Testing Your Changes

Before submitting a Pull Request:
1. Build all targets: `cmake --build build --config Release --parallel`.
2. Run automated tests: `ctest --test-dir build -C Release --output-on-failure`.
3. Verify DLL outputs in `dist/Mods/`.
4. Test in-game via `cubeforge.loader`.
