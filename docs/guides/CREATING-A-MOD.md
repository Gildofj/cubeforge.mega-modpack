# Developer Guide: Creating a New Sub-Mod

**cubeforge.mega-modpack** is designed with clean modularity. Every feature module is encapsulated in its own standalone directory deriving from `BaseMod` and configured with `add_cubeforge_mod()`.

---

## 1. Sub-Mod Lifecycle & `BaseMod` Interface

All sub-mods inherit from `BaseMod` (`src/core/BaseMod.h`), which inherits from `GenericMod`.

```cpp
#pragma once
#include "BaseMod.h"

class CustomFeatureMod : public BaseMod
{
public:
    CustomFeatureMod()
        : BaseMod("Custom Feature Mod", "cubeforge-custom-feature", 13, { 1, 0, 0 })
    {
    }

    // Called once during startup
    virtual void Initialize() override;

    // Called every frame
    virtual void OnGameTick(cube::Game* game) override;

    // Called when the player enters a chat command
    virtual int OnChat(std::wstring* message) override;

    // Called when DirectInput keyboard state is polled
    virtual void OnGetKeyboardState(BYTE* diKeys) override;
};
```

---

## 2. Step-by-Step Implementation

### Step 1: Create Module Directory
Create a folder under `src/mods/custom_feature/`:
- `CustomFeatureMod.h`
- `CustomFeatureMod.cpp`
- `CMakeLists.txt`

### Step 2: Configure `CMakeLists.txt`
In `src/mods/custom_feature/CMakeLists.txt`:
```cmake
add_cubeforge_mod(custom-feature
    SOURCES
        CustomFeatureMod.cpp
    HEADERS
        CustomFeatureMod.h
)
```

### Step 3: Register in `src/mods/CMakeLists.txt`
```cmake
add_subdirectory(custom_feature)
```
