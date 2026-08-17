# Developer Guide: Creating a New Sub-Mod

CubeMegaMod is designed to be easily extensible. Every feature module is encapsulated in its own class deriving from `CubeMod`. This guide explains how to create, register, and hook a new sub-mod.

---

## 1. Sub-Mod Lifecycle & `CubeMod` Interface

All sub-mods inherit from `CubeMod` (`src/CubeMod.h`), which inherits from CWSDK's `GenericMod`.

### Key Virtual Methods

```cpp
#pragma once
#include "../../CubeMod.h"

class CustomFeatureMod : public CubeMod
{
public:
    CustomFeatureMod() {
        m_Name = "Custom Feature Mod";
        m_FileName = "CustomFeatureMod";
        m_ID = 13; // Next available ID
        m_Version = { 1, 0, 0 };
        m_Enabled = true;
    }

    // Called once during startup
    virtual void Initialize() override;

    // Called every frame
    virtual void OnGameTick(cube::Game* game) override;

    // Called when the player enters a chat command
    virtual int OnChat(std::wstring* message) override;

    // Called when DirectInput keyboard state is polled
    virtual void OnGetKeyboardState(BYTE* diKeys) override;

    // Custom CubeMegaMod events:
    virtual int OnChestInteraction(cube::Game* game, cube::Creature* creature, int type) override;
    virtual void OnLoreIncrease(cube::Game* game, int value) override;
    virtual int OnCreatureTalk(cube::Game* game, cube::Creature* creature) override;
    virtual void OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker) override;
    virtual int OnShopInteraction(cube::Game* game, std::vector<std::vector<cube::ItemStack>>* itemVector, int classType, long long id) override;
};
```

---

## 2. Step-by-Step Implementation

### Step 1: Create Module Directory
Create a folder under `src/mods/CustomFeatureMod/`:
- `CustomFeatureMod.h`
- `CustomFeatureMod.cpp`

### Step 2: Implement Logic (AAA Pattern Example)
```cpp
#include "CustomFeatureMod.h"

void CustomFeatureMod::Initialize()
{
    // Arrange: Perform memory patches or register custom hooks
    // Act: Write byte to memory offset
    // WriteByte(CWOffset(0x123456), 0x90);
}

void CustomFeatureMod::OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker)
{
    // Arrange
    cube::Creature* player = cube::GetGame()->GetPlayer();
    if (!player || attacker != player) return;

    // Act
    cube::Helper::DropItem(player, cube::Helper::ItemGenerationType::Gold, 10);
    cube::GetGame()->PrintMessage(L"[Custom Mod] Bonus 10 gold rewarded!\n", 100, 255, 100);
}
```

### Step 3: Register in `main.cpp`
Include your header and instantiate your mod in `main.cpp`:

```cpp
#include "src/mods/CustomFeatureMod/CustomFeatureMod.h"

// Inside Mod::Initialize():
modVector.push_back(new CustomFeatureMod());
```

### Step 4: Update Build Configuration
Run the CMake generator script to automatically append your new files:

```bash
python GenerateProjectCMake.py
```

---

## 3. Best Practices
1. **Never hardcode memory pointers**: Always use `CWBase()` and `CWOffset(offset)` from CWSDK to calculate absolute memory locations relative to the game's base image.
2. **Check for GUI / Host running state**: In `OnGameTick`, always verify `!cube::Helper::InGUI(game)` and `game->host.running` before performing entity modifications.
3. **Register chat commands**: Provide toggle commands or status reporting in `OnChat` to allow runtime configuration.
