# ==============================================================================
# cubeforge-player-updates - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(player-updates
    SOURCES
        PlayerUpdatesMod.cpp
        classes/MonkClass.cpp
    HEADERS
        PlayerUpdatesMod.h
        CharacterClass.h
        classes/MonkClass.h
        abilities/FullHealAbility.h
        abilities/HealingPuddle.h
        abilities/LavaPuddle.h
        abilities/PoisonPuddle.h
    ASM_SOURCES
        asm/hooks_player.asm
)
