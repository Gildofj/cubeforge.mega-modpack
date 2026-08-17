# Building CubeMegaMod from Source

This guide covers how to set up the toolchain and compile **CubeMegaMod** into a standalone x64 dynamic link library (`.dll`).

---

## 1. Prerequisites & Toolchain

Cube World mods use custom calling conventions and naked assembly routines, requiring the LLVM/Clang compiler targeting MSVC x64.

1. **Visual Studio 2019 / 2022**:
   - Workload: **Desktop development with C++**
   - Optional Components:
     - `C++ Clang Compiler for Windows` (LLVM toolset)
     - `C++ CMake tools for Windows`
     - `Windows 10/11 SDK`
2. **Git** (with submodule support)
3. **Python 3.x** (optional, for running CMake helper scripts)

---

## 2. Cloning the Repository

Make sure to clone recursively to pull `cwmods` (Cube World SDK):

```bash
git clone --recurse-submodules https://github.com/Nichiren/CubeMegaMod.git
cd CubeMegaMod
```

If you already cloned without submodules:
```bash
git submodule update --init --recursive
```

---

## 3. Building with Visual Studio (Recommended)

1. Open **Visual Studio 2019 / 2022**.
2. Select **Open a local folder** and choose the `CubeMegaMod` root directory.
3. In the configuration dropdown, select **`x64-Clang-Release`** (or `x64-Clang-Debug`).
   - If the configuration is not present, configure `CMakeSettings.json` to target Clang x64.
4. Open the CMake target dropdown, choose `CubeMegaMod-v1.5.9.dll`.
5. Click **Build -> Build All** (or press `Ctrl + Shift + B`).
6. The compiled binary will be placed under:
   ```text
   out/build/x64-Clang-Release/CubeMegaMod-v1.5.9.dll
   ```

---

## 4. Building via Command Line (CMake + Ninja)

From an `x64 Native Tools Command Prompt for VS`:

```bash
# 1. Create build directory
mkdir build
cd build

# 2. Configure CMake with Clang toolset
cmake .. -G "Ninja" -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Release

# 3. Compile the DLL target
cmake --build . --config Release --target CubeMegaMod-v1.5.9
```

---

## 5. Updating CMake File Lists

If you add new `.cpp` or `.h` files to `src/`:
You can automatically refresh `CMakeLists.txt` using the included Python generator script:

```bash
python GenerateProjectCMake.py
```

This scans `src/` and updates the `add_library` entry in `CMakeLists.txt`.
