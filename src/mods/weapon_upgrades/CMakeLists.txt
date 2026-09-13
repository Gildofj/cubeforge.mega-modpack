# ==============================================================================
# cubeforge-weapon-upgrades - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(weapon-upgrades
    SOURCES
        WeaponUpgradeMod.cpp
    HEADERS
        WeaponUpgradeMod.h
    ASM_SOURCES
        asm/hooks_weapon.asm
)
