# Building cubeforge.mega-modpack from Source

Este guia detalha a configuração do ambiente de desenvolvimento, pré-requisitos e procedimentos para compilar o **cubeforge.mega-modpack** gerando as 12 DLLs standalone e o pacote consolidado `cubeforge-megamod.dll`.

---

## 📑 Índice
- [1. Pré-requisitos & Ferramentas](#1-pré-requisitos--ferramentas)
- [2. Clonagem do Repositório](#2-clonagem-do-repositório)
- [3. Compilação via CMake Presets (Recomendado)](#3-compilação-via-cmake-presets-recomendado)
- [4. Compilação via Visual Studio IDE](#4-compilação-via-visual-studio-ide)
- [5. Compilação via Script Automatizado (`build.ps1`)](#5-compilação-via-script-automatizado-buildps1)
- [6. Execução da Suite de Testes (CTest)](#6-execução-da-suite-de-testes-ctest)
- [7. Estrutura de Artefatos Gerados](#7-estrutura-de-artefatos-gerados)
- [8. Resolução de Problemas Comuns](#8-resolução-de-problemas-comuns)

---

## 1. Pré-requisitos & Ferramentas

1. **Visual Studio 2022 (v143) ou Visual Studio 2026 (v145 / Dev18)**:
   - Carga de trabalho: **Desenvolvimento para Desktop com C++**
   - Componentes individuais obrigatórios:
     - `Ferramentas de build do MSVC C++ x64/x86`
     - `Ferramentas do CMake em C++ para Windows`
     - `SDK do Windows 10/11 (10.0.22621+ ou superior)`
     - `Suporte ao Compilador MASM (Macro Assembler x64 - ml64.exe)`
2. **Ninja Build System** (geralmente instalado automaticamente pelo Visual Studio ou via winget: `winget install Ninja-build.Ninja`).
3. **CMake 3.25 ou superior**.
4. **Git**.

---

## 2. Clonagem do Repositório

Clone o repositório recursivamente ou em conjunto com os projetos irmãos do ecossistema:

```powershell
git clone https://github.com/Gildofj/cubeforge.mega-modpack.git d:\Projects\cubeforge.mega-modpack
cd d:\Projects\cubeforge.mega-modpack
```

> [!TIP]
> Se o repositório [`cubeforge.sdk`](https://github.com/Gildofj/CWSDK) estiver clonado na pasta vizinha (`../cubeforge.sdk`), o CMake utilizará a cópia local automaticamente via `cmake/CubeForgeSDK.cmake`. Caso contrário, o CMake fará o download da versão mais recente via Git `FetchContent`.

---

## 3. Compilação via CMake Presets (Recomendado)

Abra o **x64 Native Tools Command Prompt for VS** (ou inicialize `vcvars64.bat`):

```powershell
# Carregar ambiente MSVC (se estiver no PowerShell comum)
& "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvars64.bat"

# 1. Configurar e compilar em modo Debug (com testes ativados)
cmake --preset windows-debug
cmake --build --preset windows-debug

# 2. Executar testes automatizados
ctest --preset windows-test

# 3. Compilar em modo Release de Alta Performance
cmake --preset windows-release
cmake --build --preset windows-release
```

---

## 4. Compilação via Visual Studio IDE

1. Abra o Visual Studio 2022 / 2026.
2. Selecione **Arquivo -> Abrir -> Pasta** (`Open Folder`) e aponte para `d:\Projects\cubeforge.mega-modpack`.
3. Na barra de ferramentas superior, selecione a configuração de Preset desejada:
   - **`Windows x64 Debug`** (compilação com símbolos e suite de testes)
   - **`Windows x64 Release`** (compilação otimizada para distribuição)
4. Pressione `Ctrl + Shift + B` ou clique em **Compilar -> Compilar Tudo**.

---

## 5. Compilação via Script Automatizado (`build.ps1`)

O repositório inclui um script PowerShell para automação completa de limpeza, build e validação:

```powershell
# Compilar Release padrão
.\build.ps1 -Config Release

# Compilar Debug com execução dos testes
.\build.ps1 -Config Debug -Test

# Limpeza total dos diretórios de saída e reconstrução
.\build.ps1 -Clean -Config Release
```

---

## 6. Execução da Suite de Testes (CTest)

A suite de testes unitários e de integração valida a integridade matemática, parsing binário e orquestração do modpack:

```powershell
# Execução via CTest com saída detalhada em caso de falha
ctest --preset windows-test --output-on-failure
```

---

## 7. Estrutura de Artefatos Gerados

Após a compilação, todos os binários estarão organizados no diretório `dist/Mods/`:

```text
dist/
└── Mods/
    ├── cubeforge-beginner-mode.dll
    ├── cubeforge-combat-updates.dll
    ├── cubeforge-creature-updates.dll
    ├── cubeforge-lore-interactions.dll
    ├── cubeforge-player-updates.dll
    ├── cubeforge-quest-system.dll
    ├── cubeforge-region-lock.dll
    ├── cubeforge-sea-exploration.dll
    ├── cubeforge-shop-updates.dll
    ├── cubeforge-stack-updates.dll
    ├── cubeforge-weapon-upgrades.dll
    ├── cubeforge-world-gen.dll
    └── cubeforge-megamod.dll         <-- Pacote completo consolidado
```

---

## 8. Resolução de Problemas Comuns

### Erro: `The C compiler identification is unknown` / `cl is not found in PATH`
- **Causa**: CMake executado em prompt do PowerShell padrão sem as variáveis de ambiente do compilador MSVC carregadas.
- **Solução**: Execute o comando a partir do **x64 Native Tools Command Prompt** do Visual Studio ou chame `vcvars64.bat`.

### Erro: `MSVC Toolset 'v143' not found` no Visual Studio 2026 (Dev18)
- **Causa**: Presets configurados com ferramenta legada fixa.
- **Solução**: Os presets utilizam `"toolset": { "value": "host=x64" }`, garantindo seleção dinâmica do MSVC instalado no sistema.

