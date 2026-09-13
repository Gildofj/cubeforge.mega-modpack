# ==============================================================================
# CubeForgeMod.cmake - Macro e Helpers para Compilação dos Mods CubeForge
# ==============================================================================

function(add_cubeforge_mod MOD_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SOURCES HEADERS ASM_SOURCES LIBS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(TARGET_NAME "cubeforge-${MOD_NAME}")

    # Coleta todas as fontes
    set(ALL_SOURCES ${ARG_SOURCES} ${ARG_HEADERS} ${ARG_ASM_SOURCES})

    add_library(${TARGET_NAME} SHARED ${ALL_SOURCES})

    # Definições obrigatórias para CWSDK / ModLoader
    target_compile_definitions(${TARGET_NAME} PRIVATE
        MODLOADER
        _ITERATOR_DEBUG_LEVEL=0
        DIRECTINPUT_VERSION=0x0800
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
    )

    # Diretórios de include
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/core
    )

    # Links com Core e SDK
    target_link_libraries(${TARGET_NAME} PRIVATE
        cubeforge-core
        CWSDK
        ${ARG_LIBS}
    )

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
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
        set_property(TARGET ${TARGET_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()

    # Configuração de propriedades de saída
    set_target_properties(${TARGET_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        OUTPUT_NAME "cubeforge-${MOD_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/Mods"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/dist/Mods"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/dist/Mods"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dist/Mods"
        FOLDER "Mods"
    )

    if(ARG_ASM_SOURCES)
        enable_language(ASM_MASM)
        set_source_files_properties(${ARG_ASM_SOURCES} PROPERTIES
            LANGUAGE ASM_MASM
        )
    endif()
endfunction()
