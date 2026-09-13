# ==============================================================================
# cubeforge-sea-exploration - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(sea-exploration
    SOURCES
        SeaExplorationMod.cpp
        DivingEvent.cpp
    HEADERS
        SeaExplorationMod.h
        DivingEvent.h
    ASM_SOURCES
        asm/hooks_sea.asm
)
