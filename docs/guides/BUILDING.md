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

---

## 3. Quick Automated Build (1-Command)

Você pode compilar o mod inteiro com apenas **um comando** a partir da raiz do repositório:

### Opção A: Usando `make` (Recomendado)
```bash
# Compila a DLL e copia automaticamente para a pasta dist/
make

# Executa todos os testes unitários
make test

# Limpa os diretórios de build e dist
make clean
```

### Opção B: Usando o Script PowerShell / Batch
```powershell
# No PowerShell ou Prompt de Comando
.\build.bat

# Ou com testes automatizados:
.\build.bat -Test

# Limpeza completa:
.\build.bat -Clean
```

---

## 4. Building with Visual Studio

1. Open **Visual Studio 2019 / 2022**.
2. Select **Open a local folder** and choose the `CubeMegaMod` root directory.
3. In the configuration dropdown, select **`x64-Clang-Release`** (or `x64-Release`).
4. Click **Build -> Build All** (or press `Ctrl + Shift + B`).

---

## 5. Manual Build via Command Line (CMake)

```bash
cmake -B build -A x64
cmake --build build --config Release
```

---

## 5. Updating CMake File Lists

If you add new `.cpp` or `.h` files to `src/`:
You can automatically refresh `CMakeLists.txt` using the included Python generator script:

```bash
python GenerateProjectCMake.py
```

This scans `src/` and updates the `add_library` entry in `CMakeLists.txt`.
