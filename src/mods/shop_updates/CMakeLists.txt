# ==============================================================================
# cubeforge-shop-updates - CMakeLists.txt
# ==============================================================================

add_cubeforge_mod(shop-updates
    SOURCES
        ShopUpdateMod.cpp
    HEADERS
        ShopUpdateMod.h
    ASM_SOURCES
        asm/hooks_shop.asm
)
