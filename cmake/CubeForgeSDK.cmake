# ==============================================================================
# CubeForgeSDK.cmake - Integração do SDK via FetchContent / Caminho Local
# ==============================================================================
include(FetchContent)

set(CUBEFORGE_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cubeforge.sdk" CACHE PATH "Caminho local opcional para cubeforge.sdk")

if(EXISTS "${CUBEFORGE_SDK_PATH}/CMakeLists.txt")
    message(STATUS "[CubeForge] Usando CubeForge SDK local: ${CUBEFORGE_SDK_PATH}")
    FetchContent_Declare(
        cubeforge_sdk
        SYSTEM
        SOURCE_DIR "${CUBEFORGE_SDK_PATH}"
    )
else()
    message(STATUS "[CubeForge] Buscando CubeForge SDK via Git FetchContent...")
    FetchContent_Declare(
        cubeforge_sdk
        SYSTEM
        GIT_REPOSITORY https://github.com/Gildofj/cubeforge.sdk.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
    )
endif()

FetchContent_MakeAvailable(cubeforge_sdk)
