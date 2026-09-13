# ==============================================================================
# cubeforge-lore-interactions - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(lore-interactions
    SOURCES
        LoreInteractionMod.cpp
    HEADERS
        LoreInteractionMod.h
    ASM_SOURCES
        asm/hooks_lore.asm
)
