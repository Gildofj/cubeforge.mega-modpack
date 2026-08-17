#include "../test_framework.h"
#include "src/CubeMod.h"
#include "src/cwsdk-extension/helper/FileParser.h"
#include <vector>

// Custom mock sub-mods for testing configuration and settings parsing
class MockExplorationMod : public CubeMod {
public:
    MockExplorationMod() {
        m_Name = "Mock Sea Exploration Mod";
        m_FileName = "MockSeaExploration";
        m_ID = 101;
        m_Version = { 1, 2, 3 };
        m_Enabled = true;
    }
};

class MockCombatMod : public CubeMod {
public:
    MockCombatMod() {
        m_Name = "Mock Combat Update Mod";
        m_FileName = "MockCombatUpdate";
        m_ID = 102;
        m_Version = { 2, 0, 1 };
        m_Enabled = false;
    }
};

TEST_FUNC(ModSettingsParser, ModVersionFormatting) {
    ModVersion vDefault;
    ASSERT_EQ(vDefault.major, 1);
    ASSERT_EQ(vDefault.minor, 0);
    ASSERT_EQ(vDefault.sub, 0);
    ASSERT_STREQ(vDefault.ToString().c_str(), "v1.0.0");

    ModVersion vCustom(2, 5, 12);
    ASSERT_EQ(vCustom.major, 2);
    ASSERT_EQ(vCustom.minor, 5);
    ASSERT_EQ(vCustom.sub, 12);
    ASSERT_STREQ(vCustom.ToString().c_str(), "v2.5.12");
}

TEST_FUNC(ModSettingsParser, CubeModDefaultsAndState) {
    CubeMod baseMod;
    ASSERT_STREQ(baseMod.m_Name.c_str(), "Standard Mod Class");
    ASSERT_EQ(baseMod.m_ID, 0);
    ASSERT_TRUE(baseMod.m_Enabled);
    ASSERT_STREQ(baseMod.m_Version.ToString().c_str(), "v1.0.0");

    MockExplorationMod expMod;
    ASSERT_EQ(expMod.m_ID, 101);
    ASSERT_TRUE(expMod.m_Enabled);
    ASSERT_STREQ(expMod.m_Version.ToString().c_str(), "v1.2.3");

    MockCombatMod combatMod;
    ASSERT_EQ(combatMod.m_ID, 102);
    ASSERT_FALSE(combatMod.m_Enabled);
    ASSERT_STREQ(combatMod.m_Version.ToString().c_str(), "v2.0.1");
}

TEST_FUNC(ModSettingsParser, SaveAndApplySettingsRoundtrip) {
    MockExplorationMod mod1;
    MockCombatMod mod2;

    mod1.m_Enabled = false; // Changed from default
    mod2.m_Enabled = true;  // Changed from default

    std::vector<CubeMod*> modVector = { &mod1, &mod2 };

    // Save to binary file
    cube::SaveSettings(&modVector);

    // Reset mod enabled states to defaults to test if ApplySettings restores them
    mod1.m_Enabled = true;
    mod2.m_Enabled = false;

    // Apply settings from the persisted file
    cube::ApplySettings(&modVector);

    ASSERT_FALSE(mod1.m_Enabled);
    ASSERT_TRUE(mod2.m_Enabled);
}

void RegisterModSettingsParserTests() {
    REGISTER_TEST(ModSettingsParser, ModVersionFormatting);
    REGISTER_TEST(ModSettingsParser, CubeModDefaultsAndState);
    REGISTER_TEST(ModSettingsParser, SaveAndApplySettingsRoundtrip);
}
