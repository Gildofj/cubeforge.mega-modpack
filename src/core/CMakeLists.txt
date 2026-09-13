# ==============================================================================
# src/core/CMakeLists.txt - Biblioteca Estática cubeforge-core
# ==============================================================================

set(CORE_SOURCES
    BaseMod.cpp
    BaseMod.h
    CommandHandler.cpp
    CommandHandler.h
    memory/MemoryHelper.cpp
    memory/MemoryHelper.h
    input/DButton.cpp
    input/DButton.h
    utils/GameHelper.cpp
    utils/GameHelper.h
    creature/Creature.h
    creature/CreatureFactory.cpp
    creature/CreatureFactory.h
    inventory/Inventory.cpp
    inventory/Inventory.h
    inventory/Item.h
    inventory/Timer.h
    abilities/Ability.cpp
    abilities/Ability.h
    abilities/ConvertMTSAbility.h
    abilities/FarJumpAbility.cpp
    abilities/FarJumpAbility.h
    abilities/HealAbility.cpp
    abilities/HealAbility.h
    abilities/Event.cpp
    abilities/Event.h
    abilities/EventList.cpp
    abilities/EventList.h
    abilities/AddGoldEvent.cpp
    abilities/AddGoldEvent.h
)

add_library(cubeforge-core STATIC ${CORE_SOURCES})

target_compile_definitions(cubeforge-core PUBLIC
    MODLOADER
    _ITERATOR_DEBUG_LEVEL=0
    DIRECTINPUT_VERSION=0x0800
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
)

target_include_directories(cubeforge-core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(cubeforge-core PUBLIC CWSDK)

if(MSVC)
    target_compile_options(cubeforge-core PRIVATE
        /W4
        /utf-8
        /permissive-
        /Zc:__cplusplus
        /wd4828
        /wd4100
        /MP
        /Zc:inline
    )
    set_property(TARGET cubeforge-core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set_target_properties(cubeforge-core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    FOLDER "Core"
)
