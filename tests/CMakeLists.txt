# ==============================================================================
# tests/CMakeLists.txt - Executável de Testes Unitários e Integração CubeForge
# ==============================================================================

set(TEST_SOURCES
    "main.cpp"
    "test_framework.h"
    "unit/test_simplex_noise.cpp"
    "unit/test_timer_and_events.cpp"
    "unit/test_mod_settings_parser.cpp"
    "unit/test_abilities_and_classes.cpp"
    "unit/test_quest_and_inventory.cpp"
    "integration/test_megamod_lifecycle.cpp"

    "${CMAKE_SOURCE_DIR}/src/mods/world_gen/SimplexNoise.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/world_gen/WorldGenMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/quest_system/Quest.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/quest_system/QuestMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/player_updates/classes/MonkClass.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/player_updates/PlayerUpdatesMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/sea_exploration/SeaExplorationMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/sea_exploration/DivingEvent.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/lore_interactions/LoreInteractionMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/combat_updates/CombatUpdateMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/creature_updates/CreatureUpdatesMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/shop_updates/ShopUpdateMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/beginner_mode/BeginnerModeMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/region_lock/RegionLockMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/weapon_upgrades/WeaponUpgradeMod.cpp"
    "${CMAKE_SOURCE_DIR}/src/mods/stack_updates/StackUpdatesMod.cpp"
)

set(TEST_ASM_SOURCES
    "${CMAKE_SOURCE_DIR}/src/modpack/asm/hooks_megamod.asm"
)

enable_language(ASM_MASM)
set_source_files_properties(${TEST_ASM_SOURCES} PROPERTIES
    LANGUAGE ASM_MASM
)

add_executable(cubeforge_tests
    ${TEST_SOURCES}
    ${TEST_ASM_SOURCES}
)

target_compile_definitions(cubeforge_tests PRIVATE
    MODLOADER
    MODPACK_BUILD
    _ITERATOR_DEBUG_LEVEL=0
    DIRECTINPUT_VERSION=0x0800
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
)

target_include_directories(cubeforge_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/mods
    ${CMAKE_SOURCE_DIR}/src/mods/sea_exploration
    ${CMAKE_SOURCE_DIR}/src/mods/lore_interactions
    ${CMAKE_SOURCE_DIR}/src/mods/combat_updates
    ${CMAKE_SOURCE_DIR}/src/mods/creature_updates
    ${CMAKE_SOURCE_DIR}/src/mods/shop_updates
    ${CMAKE_SOURCE_DIR}/src/mods/world_gen
    ${CMAKE_SOURCE_DIR}/src/mods/beginner_mode
    ${CMAKE_SOURCE_DIR}/src/mods/region_lock
    ${CMAKE_SOURCE_DIR}/src/mods/weapon_upgrades
    ${CMAKE_SOURCE_DIR}/src/mods/quest_system
    ${CMAKE_SOURCE_DIR}/src/mods/player_updates
    ${CMAKE_SOURCE_DIR}/src/mods/player_updates/classes
    ${CMAKE_SOURCE_DIR}/src/mods/stack_updates
)

target_link_libraries(cubeforge_tests PRIVATE
    cubeforge-core
    CWSDK
)

if(MSVC)
    target_compile_options(cubeforge_tests PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:
            /W4
            /utf-8
            /permissive-
            /Zc:__cplusplus
            /wd4828
            /wd4100
            /MP
            /Zc:inline
        >
    )
    set_property(TARGET cubeforge_tests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_target_properties(cubeforge_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER "Tests"
)

add_test(NAME cubeforge_tests COMMAND cubeforge_tests)
