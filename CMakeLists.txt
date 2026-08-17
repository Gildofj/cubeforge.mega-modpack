cmake_minimum_required(VERSION 3.15)

project(CubeMegaMod VERSION 1.5.9 LANGUAGES CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "CubeMegaMod REQUIRES x64 architecture.")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(FATAL_ERROR "CubeMegaMod cannot be built in Debug mode due to MSVC binary compatibility issues.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(cwmods)

# Core Sources
set(CORE_SOURCES
    "main.cpp"
    "main.h"
    "src/CubeMod.h"
    "src/cwsdk-extension.h"
    "src/memory_helper/memory_helper.h"
)

# Extension Sources
set(EXTENSION_SOURCES
    "src/cwsdk-extension/ability/Ability.cpp"
    "src/cwsdk-extension/ability/Ability.h"
    "src/cwsdk-extension/ability/ConvertMTSAbility.h"
    "src/cwsdk-extension/ability/FarJumpAbility.cpp"
    "src/cwsdk-extension/ability/FarJumpAbility.h"
    "src/cwsdk-extension/ability/HealAbility.cpp"
    "src/cwsdk-extension/ability/HealAbility.h"
    "src/cwsdk-extension/button/DButton.cpp"
    "src/cwsdk-extension/button/DButton.h"
    "src/cwsdk-extension/creature/Creature.h"
    "src/cwsdk-extension/creature/CreatureFactory.cpp"
    "src/cwsdk-extension/creature/CreatureFactory.h"
    "src/cwsdk-extension/events/AddGoldEvent.cpp"
    "src/cwsdk-extension/events/AddGoldEvent.h"
    "src/cwsdk-extension/events/Event.cpp"
    "src/cwsdk-extension/events/Event.h"
    "src/cwsdk-extension/events/EventList.cpp"
    "src/cwsdk-extension/events/EventList.h"
    "src/cwsdk-extension/helper/FileParser.cpp"
    "src/cwsdk-extension/helper/FileParser.h"
    "src/cwsdk-extension/helper/Helper.cpp"
    "src/cwsdk-extension/helper/Helper.h"
    "src/cwsdk-extension/Inventory.cpp"
    "src/cwsdk-extension/Inventory.h"
    "src/cwsdk-extension/Item.h"
    "src/cwsdk-extension/quest/Quest.cpp"
    "src/cwsdk-extension/quest/Quest.h"
    "src/cwsdk-extension/Timer.h"
)

# Hook Sources
set(HOOK_SOURCES
    "src/hooks.cpp"
    "src/hooks.h"
    "src/hooks/chest_opening.h"
    "src/hooks/ChestInteractionHandler.h"
    "src/hooks/CreatureDeathHandler.h"
    "src/hooks/CreatureTalkHandler.h"
    "src/hooks/drop_gold_on_hit.h"
    "src/hooks/ItemDropPatch.h"
    "src/hooks/ItemPriceHandler.h"
    "src/hooks/lore_increase.h"
    "src/hooks/RegionLockUpdate.h"
    "src/hooks/ShopInteractionHandler.h"
)

# Mod Sources
set(MOD_SOURCES
    "src/mods/BeginnerModeMod/BeginnerModeMod.cpp"
    "src/mods/BeginnerModeMod/BeginnerModeMod.h"
    "src/mods/CombatUpdatesMod/CombatUpdateMod.cpp"
    "src/mods/CombatUpdatesMod/CombatUpdateMod.h"
    "src/mods/CreatureUpdatesMod/CreatureUpdatesMod.cpp"
    "src/mods/CreatureUpdatesMod/CreatureUpdatesMod.h"
    "src/mods/LoreInteractionMod/LoreInteractionMod.cpp"
    "src/mods/LoreInteractionMod/LoreInteractionMod.h"
    "src/mods/PlayerUpdatesMod/abilities/FullHealAbility.h"
    "src/mods/PlayerUpdatesMod/abilities/HealingPuddle.h"
    "src/mods/PlayerUpdatesMod/abilities/LavaPuddle.h"
    "src/mods/PlayerUpdatesMod/abilities/PoisonPuddle.h"
    "src/mods/PlayerUpdatesMod/classes/MonkClass.cpp"
    "src/mods/PlayerUpdatesMod/classes/MonkClass.h"
    "src/mods/PlayerUpdatesMod/CharacterClass.h"
    "src/mods/PlayerUpdatesMod/hooks/CharacterCreationHandler.h"
    "src/mods/PlayerUpdatesMod/hooks/CharacterStyleWidget_UpdateCreature.h"
    "src/mods/PlayerUpdatesMod/hooks/OnCreatureCanEquipItem.h"
    "src/mods/PlayerUpdatesMod/hooks/OnCreaturePassiveMana.h"
    "src/mods/PlayerUpdatesMod/hooks/OnExecuteAbility.h"
    "src/mods/PlayerUpdatesMod/hooks/OnGenerateCreatureAppearance.h"
    "src/mods/PlayerUpdatesMod/hooks/OnGetShiftAbilityID.h"
    "src/mods/PlayerUpdatesMod/hooks/OnGetUltimateAbilityCooldown.h"
    "src/mods/PlayerUpdatesMod/hooks/OnGetUltimateAbilityID.h"
    "src/mods/PlayerUpdatesMod/hooks/TreasureFixHandler.h"
    "src/mods/PlayerUpdatesMod/PlayerUpdatesMod.cpp"
    "src/mods/PlayerUpdatesMod/PlayerUpdatesMod.h"
    "src/mods/QuestMod/hooks/ChangeQuestDescription.h"
    "src/mods/QuestMod/hooks/ChangeQuestItemName.h"
    "src/mods/QuestMod/QuestMod.cpp"
    "src/mods/QuestMod/QuestMod.h"
    "src/mods/RegionLockUpdateMod/RegionLockUpdateMod.cpp"
    "src/mods/RegionLockUpdateMod/RegionLockUpdateMod.h"
    "src/mods/SeaExplorationMod/event/DivingEvent.cpp"
    "src/mods/SeaExplorationMod/event/DivingEvent.h"
    "src/mods/SeaExplorationMod/SeaExplorationMod.cpp"
    "src/mods/SeaExplorationMod/SeaExplorationMod.h"
    "src/mods/ShopUpdateMod/ShopUpdateMod.cpp"
    "src/mods/ShopUpdateMod/ShopUpdateMod.h"
    "src/mods/StackUpdatesMod/StackUpdatesMod.cpp"
    "src/mods/StackUpdatesMod/StackUpdatesMod.h"
    "src/mods/WeaponUpgradeMod/hooks/ModifyWeaponUpgrade.h"
    "src/mods/WeaponUpgradeMod/hooks/SmithyInteraction.h"
    "src/mods/WeaponUpgradeMod/WeaponUpgradeMod.cpp"
    "src/mods/WeaponUpgradeMod/WeaponUpgradeMod.h"
    "src/mods/WorldGenMod/Noise/SimplexNoise.cpp"
    "src/mods/WorldGenMod/Noise/SimplexNoise.h"
    "src/mods/WorldGenMod/WorldGenMod.cpp"
    "src/mods/WorldGenMod/WorldGenMod.h"
    "src/mods/WorldGenMod/WorldGenOverride/BiomeTypeOverwrite.h"
    "src/mods/WorldGenMod/WorldGenOverride/BuildingTypeOverwrite.h"
)

add_library(CubeMegaMod-v1.5.9 SHARED
    ${CORE_SOURCES}
    ${EXTENSION_SOURCES}
    ${HOOK_SOURCES}
    ${MOD_SOURCES}
)

target_link_libraries(CubeMegaMod-v1.5.9 PUBLIC CWSDK)

target_include_directories(CubeMegaMod-v1.5.9 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cwmods>
)

# Unit Testing
option(BUILD_TESTING "Build the CubeMegaMod unit test suite" ON)
if(BUILD_TESTING)
    enable_testing()
    
    file(GLOB_RECURSE MEGAMOD_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h"
    )

    if(MEGAMOD_TEST_SOURCES)
        add_executable(cubemegamod_tests 
            ${MEGAMOD_TEST_SOURCES}
            ${EXTENSION_SOURCES}
            ${HOOK_SOURCES}
            ${MOD_SOURCES}
        )
        target_link_libraries(cubemegamod_tests PRIVATE CWSDK)
        target_include_directories(cubemegamod_tests PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
            ${CMAKE_CURRENT_SOURCE_DIR}/cwmods
        )
        target_compile_definitions(cubemegamod_tests PUBLIC MODLOADER)
        add_test(NAME cubemegamod_tests COMMAND cubemegamod_tests)
    endif()
endif()