# Installation Guide

This guide will walk you through installing and configuring **CubeMegaMod** for your Cube World Steam installation.

---

## 1. Prerequisites

Before installing CubeMegaMod, ensure you have:
1. **Cube World (Steam Version)** installed and updated.
2. **Cube World Mod Launcher** by ChrisMiuchiz:
   - Download the latest launcher from [Cube World Mod Launcher Releases](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher/releases).
3. **Microsoft Visual C++ Redistributable (x64)** installed.

> [!CAUTION]
> **Always back up your save files!**
> Modded content can alter character data or inventory formats.
> Backup path: `%LOCALAPPDATA%\CubeWorld\Save\` or `<CubeWorldFolder>\Save\`.

---

## 2. Installation Steps

### Step 1: Install the Mod Launcher
1. Extract the contents of `CubeModLoader.zip` into your primary Cube World game directory (where `cubeworld.exe` is located).
2. Ensure you have the `Mods/` subfolder created inside your game directory.

### Step 2: Download CubeMegaMod
1. Download the latest `CubeMegaMod-vX.X.X.dll` from the [GitHub Releases page](https://github.com/Nichiren/CubeMegaMod/releases).

### Step 3: Copy DLL to Mods Folder
1. Move `CubeMegaMod-vX.X.X.dll` into your `<CubeWorldFolder>/Mods/` folder.
   ```text
   Cube World/
   ├── cubeworld.exe
   ├── CubeModLoader.dll
   ├── Mods/
   │   └── CubeMegaMod-v1.5.9.dll
   ```

### Step 4: Launch the Game
1. Launch Cube World using Steam or the Mod Launcher executable.
2. Upon startup, a popup window titled **"CubeMegaMods"** will appear displaying the list of active mods and their versions.
3. Click **OK** to enter the game.

---

## 3. Configuration & Verifying Installation

- Open the in-game chat by pressing `Enter`.
- Type `/mod 7 1` and press `Enter`. A popup message `Enabled: Beginner Mode Mod` confirms that the mod is active and responding.
- For all available commands, refer to the [Commands Reference](file:///d:/Projects/CubeMegaMod/docs/COMMANDS.md).

---

## 4. Troubleshooting & FAQ

### The game crashes on startup
- **Cause**: Incompatible mod loader or missing Visual C++ runtimes.
- **Solution**: Install the latest Visual C++ x64 redistributables and ensure no conflicting DLLs exist in `Mods/`.

### No popup appears on launch
- **Cause**: CubeModLoader is not hooked or `CubeMegaMod.dll` is in the wrong directory.
- **Solution**: Verify `CubeModLoader.dll` is in the root game directory alongside `cubeworld.exe` and `CubeMegaMod.dll` is in `Mods/`.

### NPCs or interactable items stop responding after long sessions
- **Cause**: Known memory/state bug in prolonged sessions.
- **Solution**: Save and restart the game client.
