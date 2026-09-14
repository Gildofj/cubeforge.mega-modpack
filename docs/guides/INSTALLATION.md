# Installation Guide: cubeforge.mega-modpack

Este guia descreve o procedimento de instalação, configuração e validação do **cubeforge.mega-modpack** na sua instalação do **Cube World (Steam x64)**.

---

## 📑 Índice
- [1. Pré-requisitos](#1-pré-requisitos)
- [2. Escolha do Modo de Instalação](#2-escolha-do-modo-de-instalação)
  - [Opção A: Pacote Consolidado (Recomendado)](#opção-a-pacote-consolidado-recomendado)
  - [Opção B: Módulos Standalone Individuais](#opção-b-módulos-standalone-individuais)
- [3. Backup e Segurança de Saves](#3-backup-e-segurança-de-saves)
- [4. Verificação no Jogo & Comandos](#4-verificação-no-jogo--comandos)
- [5. Resolução de Conflitos](#5-resolução-de-conflitos)

---

## 1. Pré-requisitos

Antes de iniciar a instalação, certifique-se de possuir:
1. **Cube World (Steam x64)** instalado e funcional.
2. Injetor de ModLoader compatível:
   - **CubeForgeLoader (`cubeforge.loader`)** (recomendado para suporte nativo a GUI ImGui e hot-reload)
   - **Cube World Mod Launcher** (ChrisMiuchiz).
3. **Microsoft Visual C++ Redistributable 2015–2022/2026 (x64)**.

---

## 2. Escolha do Modo de Instalação

Acesse a pasta raiz da sua instalação do Cube World (ex: `C:\Program Files (x86)\Steam\steamapps\common\Cube World\`). Se a pasta `Mods/` não existir, crie-a.

### Opção A: Pacote Consolidado (Recomendado)
Para desfrutar de todas as melhorias e mecânicas com o mínimo consumo de memória e orquestração centralizada:
1. Obtenha o arquivo **`cubeforge-megamod.dll`**.
2. Copie `cubeforge-megamod.dll` para a pasta `<CubeWorld>/Mods/`.
3. Inicie o jogo através do **CubeForgeLoader** / Launcher.

### Opção B: Módulos Standalone Individuais
Se você prefere instalar apenas funcionalidades isoladas, selecione apenas as DLLs desejadas da lista e copie para `<CubeWorld>/Mods/`:
- `cubeforge-sea-exploration.dll`
- `cubeforge-lore-interactions.dll`
- `cubeforge-combat-updates.dll`
- `cubeforge-creature-updates.dll`
- `cubeforge-shop-updates.dll`
- `cubeforge-world-gen.dll`
- `cubeforge-beginner-mode.dll`
- `cubeforge-region-lock.dll`
- `cubeforge-weapon-upgrades.dll`
- `cubeforge-quest-system.dll`
- `cubeforge-player-updates.dll`
- `cubeforge-stack-updates.dll`

> [!WARNING]
> **Nunca utilize o `cubeforge-megamod.dll` e as DLLs standalone ao mesmo tempo na pasta `Mods/`!**
> Isso causará injeções duplicadas de hooks nos mesmos endereços de memória do jogo.

---

## 3. Backup e Segurança de Saves

> [!CAUTION]
> **Sempre faça backup de seus personagens e mundos antes de ativar mods!**
> 
> Caminho dos saves:
> - `%LOCALAPPDATA%\CubeWorld\Save\` ou
> - `<CubeWorldFolder>\Save\`

---

## 4. Verificação no Jogo & Comandos

1. Inicie o jogo e carregue ou crie um personagem.
2. Pressione `Enter` para abrir o chat do jogo.
3. Digite `/cubeforge status`. Uma mensagem confirmará a quantidade de módulos ativos e a versão em execução.
4. Digite `/cubeforge list` para inspecionar os submódulos carregados.

Para a lista completa de comandos interativos, consulte o [Manual de Comandos](file:///d:/Projects/cubeforge.mega-modpack/docs/COMMANDS.md).

---

## 5. Resolução de Conflitos

- **O jogo não inicia ou fecha logo após a tela inicial**:
  - Verifique se você não possui versões antigas do `CubeMegaMod.dll` na pasta `Mods/`.
  - Certifique-se de que o Visual C++ Redistributable x64 está instalado.
- **Configurações resetam ao fechar o jogo**:
  - Verifique se a pasta `Mods/` tem permissões de escrita para salvar os arquivos de estado `.sav`.
