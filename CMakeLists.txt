cmake_minimum_required(VERSION 3.25)

cmake_policy(SET CMP0091 NEW) # Use MSVC_RUNTIME_LIBRARY property

project(cubeforge.mega-modpack VERSION 2.0.0 LANGUAGES C CXX ASM_MASM)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "cubeforge.mega-modpack REQUIRES x64 architecture.")
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Integração SDK CubeForge (FetchContent com fallback local)
include(cmake/CubeForgeSDK.cmake)

# Macro para compilação modular dos mods
include(cmake/CubeForgeMod.cmake)

# Sub-diretórios do projeto
add_subdirectory(src/core)
add_subdirectory(src/mods)
add_subdirectory(src/modpack)

# Testes Automatizados
option(BUILD_TESTING "Build the cubeforge.mega-modpack unit test suite" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()