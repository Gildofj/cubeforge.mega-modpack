# Building cubeforge.mega-modpack from Source

This guide covers how to set up the toolchain and compile **cubeforge.mega-modpack** into 12 standalone sub-mod DLLs and the aggregated bundle DLL.

---

## 1. Prerequisites & Toolchain

1. **Visual Studio 2022** (v143 toolset):
   - Workload: **Desktop development with C++**
   - Components:
     - `MSVC v143 - VS 2022 C++ x64/x86 build tools`
     - `C++ CMake tools for Windows`
     - `Windows 10/11 SDK`
2. **Git**
3. **CMake 3.25+**

---

## 2. Cloning the Repository

```bash
git clone https://github.com/Gildofj/cubeforge.mega-modpack.git
cd cubeforge.mega-modpack
```

---

## 3. Building via CMake (Command Line)

```powershell
# Configure CMake for x64
cmake -B build -A x64

# Build all 13 DLL targets and the test executable
cmake --build build --config Release --parallel

# Run CTest verification suite
ctest --test-dir build -C Release --output-on-failure
```

All 13 output DLLs (`cubeforge-<nome>.dll` and `cubeforge-megamod.dll`) will be generated inside `dist/Mods/`.
