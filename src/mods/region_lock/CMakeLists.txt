# ==============================================================================
# cubeforge-region-lock - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(region-lock
    SOURCES
        RegionLockMod.cpp
    HEADERS
        RegionLockMod.h
    ASM_SOURCES
        asm/hooks_regionlock.asm
)
