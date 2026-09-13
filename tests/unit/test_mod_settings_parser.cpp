#include "../test_framework.h"
#include "BaseMod.h"
#include <vector>

// Custom mock sub-mods for testing configuration and settings parsing
class MockExplorationMod : public BaseMod {
public:
    struct Config {
        bool autoGold;
        int divingLevel;
    } config;

    MockExplorationMod()
        : BaseMod("Mock Sea Exploration Mod", "MockSeaExploration", 101, { 1, 2, 3 })
    {
        config = { true, 5 };
        m_Enabled = true;
    }
};

class MockCombatMod : public BaseMod {
public:
    struct Config {
        bool doubleTap;
        float dodgeSpeed;
    } config;

    MockCombatMod()
        : BaseMod("Mock Combat Update Mod", "MockCombatUpdate", 102, { 2, 0, 1 })
    {
        config = { false, 1.5f };
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
    BaseMod baseMod("Standard Mod Class", "BaseMod", 0, { 1, 0, 0 });
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
    mod1.config.autoGold = false;
    mod1.config.divingLevel = 10;

    // Save state to binary file
    mod1.Save(&mod1.config, sizeof(mod1.config));

    // Reset config to defaults
    mod1.config.autoGold = true;
    mod1.config.divingLevel = 1;

    // Load persisted state back
    mod1.Load(&mod1.config, sizeof(mod1.config));

    ASSERT_FALSE(mod1.config.autoGold);
    ASSERT_EQ(mod1.config.divingLevel, 10);
}

void RegisterModSettingsParserTests() {
    REGISTER_TEST(ModSettingsParser, ModVersionFormatting);
    REGISTER_TEST(ModSettingsParser, CubeModDefaultsAndState);
    REGISTER_TEST(ModSettingsParser, SaveAndApplySettingsRoundtrip);
}
