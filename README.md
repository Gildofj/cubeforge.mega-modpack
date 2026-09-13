# cubeforge.mega-modpack

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-blue.svg)](https://microsoft.com)
[![Language](https://img.shields.io/badge/Language-C%2B%2B20-00599C.svg)](https://isocpp.org)
[![Build System](https://img.shields.io/badge/Build-CMake%20%7C%20MSVC-green.svg)](https://cmake.org)
[![Target](https://img.shields.io/badge/Target-Cube%20World%20Steam-ff69b4.svg)](https://store.steampowered.com/app/1128000/Cube_World/)

**cubeforge.mega-modpack** é a reconstrução e modularização de alta performance do CubeMegaMod para o lançamento Steam de **Cube World**, integrado nativamente ao ecossistema **CubeForge** (`cubeforge.sdk` e `cubeforge.loader`). 

Ele disponibiliza **12 sub-mods independentes** (`cubeforge-<nome>.dll`) para escolha modular do usuário e o pacote consolidado **`cubeforge-megamod.dll`**.

> [!IMPORTANT]
> **Faça backup dos seus dados salvos antes de jogar com mods!**
> Os arquivos de save ficam localizados em `%LOCALAPPDATA%\CubeWorld\Save\` ou `<CubeWorldFolder>\Save\`.

---

## 📑 Índice
- [✨ Módulos Disponíveis](#-módulos-disponíveis)
- [📦 Instalação](#-instalação)
- [🎮 Comandos no Jogo](#-comandos-no-jogo)
- [🏛️ Arquitetura & Ecossistema CubeForge](#-arquitetura--ecossistema-cubeforge)
- [🛠️ Compilação via Código-Fonte](#-compilação-via-código-fonte)
- [🤝 Contribuição](#-contribuição)
- [❤️ Créditos & Agradecimentos](#-créditos--agradecimentos)

---

## ✨ Módulos Disponíveis

Você pode carregar os mods individualmente através de suas respectivas DLLs ou usar o pacote agregador:

| ID | Módulo | Binário Standalone | Descrição |
| :---: | :--- | :--- | :--- |
| **1** | **Sea Exploration** | `cubeforge-sea-exploration.dll` | Economia de oxigênio submarino (10g / 10s), 4 níveis de baús e bosses marítimos. |
| **2** | **Lore Interactions** | `cubeforge-lore-interactions.dll` | Inspeção de lore com chances de drop de itens raros e artefatos. |
| **3** | **Combat Updates** | `cubeforge-combat-updates.dll` | Conversão de Mana em Stamina (Tecla 1), Cura 50-combo (Tecla 2) e esquiva rápida via Double-Tap. |
| **4** | **Creature Updates** | `cubeforge-creature-updates.dll` | +50% em stats de pets companheiros, balanceamento de bumerangues e mages, 40 Gold inicial. |
| **5** | **Shop Updates** | `cubeforge-shop-updates.dll` | Gem Trader e Item Vendor com venda de artefatos, itens de travessia e cubos espirituais. |
| **6** | **World Generation** | `cubeforge-world-gen.dll` | Geração procedural com Simplex Noise e remoção de restrições de biomas iniciais. |
| **7** | **Beginner Mode** | `cubeforge-beginner-mode.dll` | Ajuste dinâmico de atributos de criaturas hostis nos níveis 1 a 5. |
| **8** | **Region Lock Update** | `cubeforge-region-lock.dll` | Decaimento progressivo de poder de itens por distância regional ($-2$ estrelas/região, $-1$ para itens `+`). |
| **9** | **Weapon Upgrades** | `cubeforge-weapon-upgrades.dll` | Adaptação e upgrade de armas no ferreiro (estilo Cube World Alpha). |
| **10** | **Quest System** | `cubeforge-quest-system.dll` | Quests procedurais de eliminação de mais de 30 tipos de criaturas com recompensas diretas. |
| **11** | **Player Updates** | `cubeforge-player-updates.dll` | Classe **Monk** (especializações Chieftain e Sorcerer) e 7 novas raças jogáveis. |
| **12** | **Stack Updates** | `cubeforge-stack-updates.dll` | Patch de qualidade de vida aumentando o limite máximo de stack para **100** para todos os itens. |
| **—** | **MegaModPack Bundle** | **`cubeforge-megamod.dll`** | **Pacote completo agregador com todos os 12 submódulos integrados.** |

---

## 📦 Instalação

### Opção 1: Via CubeForge Loader (Recomendado)
1. Instale o **`cubeforge.loader`**.
2. Copie os mods desejados (ou `cubeforge-megamod.dll`) para a pasta `Mods/` do seu Cube World.
3. Inicie o jogo.

### Opção 2: Via Cube World Mod Launcher Oficial
1. Extraia o [Cube World Mod Launcher](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher/releases) na pasta raiz do jogo.
2. Coloque as DLLs em `<CubeWorldFolder>/Mods/`.
3. Inicie o jogo pelo launcher.

---

## 🎮 Comandos no Jogo

Abra o chat (`Enter`) para executar os comandos do framework:

```text
/cubeforge status             - Exibe status e quantidade de mods carregados
/cubeforge list               - Lista todos os submódulos e seus estados
/cubeforge mod <id> <1/0>     - Ativa (1) ou Desativa (0) um módulo em tempo de execução
/cubeforge combat doubletap <1/0> - Alterna esquiva double-tap
/cubeforge sea autogold <1/0>     - Alterna uso automático de ouro no mergulho
```

---

## 🏛️ Arquitetura & Ecossistema CubeForge

- **Camada Core Estática (`cubeforge-core`)**: Utilitários comuns de memória (`MemoryHelper`), DirectInput (`DButton`), ciclo de vida (`BaseMod`) e roteador `/cubeforge`.
- **Compatibilidade ABI**: C++20 com runtime `/MT` (`MultiThreaded`), idêntico ao `cubeforge.sdk`.
- **Hooks Desacoplados**: Rotinas assembly MASM x64 isoladas por módulo sem colisão de endereços.

---

## 🛠️ Compilação via Código-Fonte

Pré-requisitos:
- Visual Studio 2022 (v143 ou superior) com suporte a C++ e MASM (x64)
- CMake 3.25+

```powershell
# Configurar e compilar todos os alvos em Release x64
cmake -B build -A x64
cmake --build build --config Release --parallel

# Executar suite de testes unitários e integração
ctest --test-dir build -C Release --output-on-failure
```

Os binários compilados estarão localizados em `dist/Mods/`.

---

## 🤝 Contribuição

Pull Requests e issues são bem-vindos! Siga os padrões de código estrito e arquitetura modular do repositório.

---

## ❤️ Créditos & Agradecimentos

- **ChrisMiuchiz**: Criador do CWSDK e Cube World Mod Launcher.
- **gijsgroenewegen**: Criador original do CubeMegaMod.
- **Ecossistema CubeForge**: Mantido por Gildo FJ.
- **TheBagel3, GrayShadoz, Raxiti, kuba.dlouhej**: Testes e feedback da comunidade.
