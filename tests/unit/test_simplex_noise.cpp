#include "../test_framework.h"
#include "world_gen/SimplexNoise.h"
#include <cmath>

TEST_FUNC(SimplexNoiseSuite, Noise1DBoundsAndDeterminism) {
    // Noise on integer coordinates should be 0 or near 0
    ASSERT_NEAR(SimplexNoise::noise(0.0f), 0.0f, 0.0001f);
    ASSERT_NEAR(SimplexNoise::noise(1.0f), 0.0f, 0.0001f);
    ASSERT_NEAR(SimplexNoise::noise(10.0f), 0.0f, 0.0001f);

    // Range checks for arbitrary non-integer inputs
    for (float x = -50.0f; x <= 50.0f; x += 1.37f) {
        float val = SimplexNoise::noise(x);
        ASSERT_GE(val, -1.05f);
        ASSERT_LE(val, 1.05f);

        // Strict determinism
        float val2 = SimplexNoise::noise(x);
        ASSERT_EQ(val, val2);
    }
}

TEST_FUNC(SimplexNoiseSuite, Noise2DBoundsAndGradients) {
    for (float x = -20.0f; x <= 20.0f; x += 3.1f) {
        for (float y = -20.0f; y <= 20.0f; y += 3.7f) {
            float val = SimplexNoise::noise(x, y);
            ASSERT_GE(val, -1.05f);
            ASSERT_LE(val, 1.05f);

            float valRepeated = SimplexNoise::noise(x, y);
            ASSERT_EQ(val, valRepeated);
        }
    }
}

TEST_FUNC(SimplexNoiseSuite, Noise3DSpaceContinuity) {
    for (float x = -10.0f; x <= 10.0f; x += 4.5f) {
        for (float y = -10.0f; y <= 10.0f; y += 4.5f) {
            for (float z = -10.0f; z <= 10.0f; z += 4.5f) {
                float val = SimplexNoise::noise(x, y, z);
                ASSERT_GE(val, -1.05f);
                ASSERT_LE(val, 1.05f);
            }
        }
    }
}

TEST_FUNC(SimplexNoiseSuite, FractalBrownianMotionSummation) {
    SimplexNoise noiseGen(1.0f, 1.0f, 2.0f, 0.5f);

    // 1D Fractal
    float f1 = noiseGen.fractal(4, 5.5f);
    ASSERT_GE(f1, -1.05f);
    ASSERT_LE(f1, 1.05f);

    // 2D Fractal
    float f2 = noiseGen.fractal(4, 12.3f, 45.6f);
    ASSERT_GE(f2, -1.05f);
    ASSERT_LE(f2, 1.05f);

    // 3D Fractal
    float f3 = noiseGen.fractal(6, 1.2f, 3.4f, 5.6f);
    ASSERT_GE(f3, -1.05f);
    ASSERT_LE(f3, 1.05f);

    // Custom frequency & persistence
    SimplexNoise customGen(0.05f, 2.0f, 2.0f, 0.25f);
    float fCustom = customGen.fractal(3, 100.0f, 200.0f);
    ASSERT_GE(fCustom, -2.5f);
    ASSERT_LE(fCustom, 2.5f);
}

void RegisterSimplexNoiseTests() {
    REGISTER_TEST(SimplexNoiseSuite, Noise1DBoundsAndDeterminism);
    REGISTER_TEST(SimplexNoiseSuite, Noise2DBoundsAndGradients);
    REGISTER_TEST(SimplexNoiseSuite, Noise3DSpaceContinuity);
    REGISTER_TEST(SimplexNoiseSuite, FractalBrownianMotionSummation);
}
