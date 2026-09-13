# ==============================================================================
# cubeforge-quest-system - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(quest-system
    SOURCES
        QuestMod.cpp
        Quest.cpp
    HEADERS
        QuestMod.h
        Quest.h
    ASM_SOURCES
        asm/hooks_quest.asm
)
