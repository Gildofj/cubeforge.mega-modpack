#include "../test_framework.h"
#include "BaseMod.h"
#include "beginner_mode/BeginnerModeMod.h"
#include "combat_updates/CombatUpdateMod.h"
#include "creature_updates/CreatureUpdatesMod.h"
#include "lore_interactions/LoreInteractionMod.h"
#include "player_updates/PlayerUpdatesMod.h"
#include "quest_system/QuestMod.h"
#include "region_lock/RegionLockMod.h"
#include "sea_exploration/SeaExplorationMod.h"
#include "shop_updates/ShopUpdateMod.h"
#include "stack_updates/StackUpdatesMod.h"
#include "weapon_upgrades/WeaponUpgradeMod.h"
#include "world_gen/WorldGenMod.h"

#include <set>
#include <vector>
#include <memory>

TEST_FUNC(MegaModLifecycle, SubModIDUniquenessAndRegistration) {
    std::vector<std::unique_ptr<BaseMod>> subMods;
    subMods.push_back(std::make_unique<BeginnerModeMod>());
    subMods.push_back(std::make_unique<CombatUpdateMod>());
    subMods.push_back(std::make_unique<CreatureUpdatesMod>());
    subMods.push_back(std::make_unique<LoreInteractionMod>());
    subMods.push_back(std::make_unique<PlayerUpdatesMod>());
    subMods.push_back(std::make_unique<QuestMod>());
    subMods.push_back(std::make_unique<RegionLockMod>());
    subMods.push_back(std::make_unique<SeaExplorationMod>());
    subMods.push_back(std::make_unique<ShopUpdateMod>());
    subMods.push_back(std::make_unique<StackUpdatesMod>());
    subMods.push_back(std::make_unique<WeaponUpgradeMod>());
    subMods.push_back(std::make_unique<WorldGenMod>());

    ASSERT_EQ(subMods.size(), (size_t)12);

    std::set<int> uniqueIDs;
    for (const auto& mod : subMods) {
        ASSERT_FALSE(mod->m_Name.empty());
        ASSERT_TRUE(mod->m_FileName != nullptr);
        ASSERT_GT(mod->m_ID, 0);
        ASSERT_TRUE(mod->m_Enabled);

        // Check ID uniqueness across all registered submods
        ASSERT_TRUE(uniqueIDs.find(mod->m_ID) == uniqueIDs.end());
        uniqueIDs.insert(mod->m_ID);
    }
}

TEST_FUNC(MegaModLifecycle, SubModVersionStrings) {
    BeginnerModeMod m1;
    ASSERT_STREQ(m1.m_Version.ToString().c_str(), "v1.0.0");

    ShopUpdateMod m2;
    ASSERT_STREQ(m2.m_Version.ToString().c_str(), "v1.2.0");

    CombatUpdateMod m3;
    ASSERT_STREQ(m3.m_Version.ToString().c_str(), "v1.0.0");
}

TEST_FUNC(MegaModLifecycle, BeginnerModeNullptrSafety) {
    BeginnerModeMod mod;
    float dummyStat = 100.0f;

    // Must be completely safe and no-op when pointers are null
    mod.OnCreatureArmorCalculated(nullptr, &dummyStat);
    ASSERT_EQ(dummyStat, 100.0f);

    mod.OnCreatureAttackPowerCalculated(nullptr, &dummyStat);
    ASSERT_EQ(dummyStat, 100.0f);

    mod.OnCreatureHPCalculated(nullptr, &dummyStat);
    ASSERT_EQ(dummyStat, 100.0f);
}

void RegisterMegaModLifecycleTests() {
    REGISTER_TEST(MegaModLifecycle, SubModIDUniquenessAndRegistration);
    REGISTER_TEST(MegaModLifecycle, SubModVersionStrings);
    REGISTER_TEST(MegaModLifecycle, BeginnerModeNullptrSafety);
}
