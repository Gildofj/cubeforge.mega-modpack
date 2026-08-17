#include "../test_framework.h"
#include "src/cwsdk-extension/quest/Quest.h"
#include "src/cwsdk-extension/button/DButton.h"
#include "src/cwsdk-extension/Inventory.h"

TEST_FUNC(QuestAndInventory, QuestTypeAndRarityTargets) {
    cube::Quest q1;
    q1.category = 2;
    q1.id = 0;
    q1.modifier = 0; // modifier % 4 == 0 -> Killing
    q1.rarity = 0;   // target = (0 + 1) * 10 = 10, reward = 100

    ASSERT_EQ((int)q1.GetType(), (int)cube::Quest::QuestType::Killing);
    ASSERT_EQ(q1.GetTarget(), 10);
    ASSERT_EQ(q1.GetReward().amount, 100);
    ASSERT_EQ((int)q1.GetReward().type, (int)cube::Quest::QuestRewardType::Gold);

    cube::Quest q2;
    q2.modifier = 1; // modifier % 4 == 1 -> Gathering
    q2.rarity = 4;   // Legendary: target = (4 + 1) * 10 = 50, reward = 500
    ASSERT_EQ((int)q2.GetType(), (int)cube::Quest::QuestType::Gathering);
    ASSERT_EQ(q2.GetTarget(), 50);
    ASSERT_EQ(q2.GetReward().amount, 500);

    cube::Quest q3;
    q3.modifier = 2; // Delivery
    ASSERT_EQ((int)q3.GetType(), (int)cube::Quest::QuestType::Delivery);

    cube::Quest q4;
    q4.modifier = 3; // Talk
    ASSERT_EQ((int)q4.GetType(), (int)cube::Quest::QuestType::Talk);
}

TEST_FUNC(QuestAndInventory, QuestProgressTrackingAndCompletion) {
    cube::Quest q;
    q.modifier = 0;
    q.rarity = 1; // target = 20
    q.num_spirits = 0;

    ASSERT_EQ(q.GetProgress(), 0);
    ASSERT_FALSE(q.IsCompleted());

    // Step progress
    q.IncreaseProgress();
    ASSERT_EQ(q.GetProgress(), 1);
    ASSERT_FALSE(q.IsCompleted());

    // Set progress directly
    q.SetProgress(19);
    ASSERT_EQ(q.GetProgress(), 19);
    ASSERT_FALSE(q.IsCompleted());

    q.SetProgress(20);
    ASSERT_EQ(q.GetProgress(), 20);
    ASSERT_TRUE(q.IsCompleted());

    q.SetProgress(25);
    ASSERT_TRUE(q.IsCompleted());
}

TEST_FUNC(QuestAndInventory, QuestSubTypesMapping) {
    cube::Quest q;
    // modifier = 0 * 4 + 0 -> cow (100)
    q.modifier = 0;
    ASSERT_EQ(q.GetSubType(), 100);

    // modifier = 1 * 4 + 0 -> fly (60)
    q.modifier = 4;
    ASSERT_EQ(q.GetSubType(), 60);

    // modifier = 2 * 4 + 0 -> chicken (56)
    q.modifier = 8;
    ASSERT_EQ(q.GetSubType(), 56);
}

TEST_FUNC(QuestAndInventory, InventoryTabConstants) {
    ASSERT_EQ((int)cube::Inventory::EquipmentTab, 0);
    ASSERT_EQ((int)cube::Inventory::SpecialsTab, 1);
    ASSERT_EQ((int)cube::Inventory::ItemsTab, 2);
    ASSERT_EQ((int)cube::Inventory::IngredientsTab, 3);
    ASSERT_EQ((int)cube::Inventory::PetsTab, 4);
    ASSERT_EQ((int)cube::Inventory::ArtifactsTab, 5);
}

TEST_FUNC(QuestAndInventory, DButtonInitializationAndKeyBinding) {
    cube::DButton btn(0x1E); // DIK_A
    ASSERT_EQ(btn.diKey, 0x1E);

    btn.SetKey(0x1F); // DIK_S
    ASSERT_EQ(btn.diKey, 0x1F);
}

void RegisterQuestAndInventoryTests() {
    REGISTER_TEST(QuestAndInventory, QuestTypeAndRarityTargets);
    REGISTER_TEST(QuestAndInventory, QuestProgressTrackingAndCompletion);
    REGISTER_TEST(QuestAndInventory, QuestSubTypesMapping);
    REGISTER_TEST(QuestAndInventory, InventoryTabConstants);
    REGISTER_TEST(QuestAndInventory, DButtonInitializationAndKeyBinding);
}
