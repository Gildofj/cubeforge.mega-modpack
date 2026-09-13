# ==============================================================================
# cubeforge-world-gen - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(world-gen
    SOURCES
        WorldGenMod.cpp
        SimplexNoise.cpp
    HEADERS
        WorldGenMod.h
        SimplexNoise.h
    ASM_SOURCES
        asm/hooks_worldgen.asm
)
