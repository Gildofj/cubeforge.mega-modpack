#include "../test_framework.h"
#include "player_updates/CharacterClass.h"
#include "player_updates/classes/MonkClass.h"
#include <vector>
#include <cstdint>

TEST_FUNC(AbilitiesAndClasses, CharacterClassEnumsAndNames) {
    ASSERT_EQ(CharacterClass::PassiveManaLoss, 0);
    ASSERT_EQ(CharacterClass::PassiveManaGain, 1);
    ASSERT_EQ(CharacterClass::NoManaChange, 2);

    ASSERT_EQ(CharacterClass::Ability::FullHeal, 166);
    ASSERT_EQ(CharacterClass::Ability::PuddleHeal, 167);
    ASSERT_EQ(CharacterClass::Ability::PuddleLava, 168);
    ASSERT_EQ(CharacterClass::Ability::PuddlePoison, 169);

    MonkClass monk;
    ASSERT_STREQ(monk.m_Name, "Monk");
    ASSERT_TRUE(wcscmp(monk.m_WideName, L"Monk") == 0);
}

TEST_FUNC(AbilitiesAndClasses, MonkClassSpecialization0Abilities) {
    MonkClass monk;
    std::vector<uint8_t> creatureBuffer(sizeof(cube::Creature), 0);
    cube::Creature* mockPlayer = reinterpret_cast<cube::Creature*>(creatureBuffer.data());

    // Specialization 0 (Default)
    mockPlayer->entity_data.specialization = 0;

    int ultId = monk.GetUltimateAbilityId(mockPlayer);
    ASSERT_EQ(ultId, CharacterClass::Ability::FullHeal);

    int ultCooldown = monk.GetUltimateAbilityCooldown(mockPlayer, ultId);
    ASSERT_EQ(ultCooldown, 20000);

    int shiftId = monk.GetShiftAbilityId(mockPlayer);
    ASSERT_EQ(shiftId, 146);

    // Middle mouse without stamina
    mockPlayer->stamina = 0.2f;
    int mmIdNoStamina = monk.GetMiddleMouseAbilityId(mockPlayer);
    ASSERT_EQ(mmIdNoStamina, 0);

    // Middle mouse with stamina >= 0.5
    mockPlayer->stamina = 0.8f;
    int mmIdWithStamina = monk.GetMiddleMouseAbilityId(mockPlayer);
    ASSERT_EQ(mmIdWithStamina, 50);
}

TEST_FUNC(AbilitiesAndClasses, MonkClassSpecialization1Abilities) {
    MonkClass monk;
    std::vector<uint8_t> creatureBuffer(sizeof(cube::Creature), 0);
    cube::Creature* mockPlayer = reinterpret_cast<cube::Creature*>(creatureBuffer.data());

    // Specialization 1
    mockPlayer->entity_data.specialization = 1;
    mockPlayer->entity_data.current_ability = 0;
    mockPlayer->entity_data.time_since_ability = 500.0f; // Above 250 threshold

    int ultId = monk.GetUltimateAbilityId(mockPlayer);
    ASSERT_EQ(ultId, CharacterClass::Ability::PuddleHeal);

    int ultCooldown = monk.GetUltimateAbilityCooldown(mockPlayer, ultId);
    ASSERT_EQ(ultCooldown, 30000);

    int shiftId = monk.GetShiftAbilityId(mockPlayer);
    ASSERT_EQ(shiftId, CharacterClass::Ability::PuddleLava);

    int mmId = monk.GetMiddleMouseAbilityId(mockPlayer);
    ASSERT_EQ(mmId, CharacterClass::Ability::PuddlePoison);
}

TEST_FUNC(AbilitiesAndClasses, MonkClassEquipmentRules) {
    MonkClass monk;

    // Fist weapon (Category 3, ID 4) -> Allowed
    cube::Item fistWeapon(3, 4);
    ASSERT_TRUE(monk.CanEquipItem(&fistWeapon));

    // Sword (Category 3, ID 0) -> Restricted for Monk
    cube::Item swordWeapon(3, 0);
    ASSERT_FALSE(monk.CanEquipItem(&swordWeapon));

    // Armor / Chest (Category 4, ID 0) -> Allowed
    cube::Item chestArmor(4, 0);
    ASSERT_TRUE(monk.CanEquipItem(&chestArmor));

    // Consumables / Items (Category 2, ID 1) -> Allowed
    cube::Item potion(2, 1);
    ASSERT_TRUE(monk.CanEquipItem(&potion));
}

TEST_FUNC(AbilitiesAndClasses, MonkClassManaGenerationStrategy) {
    MonkClass monk;
    std::vector<uint8_t> creatureBuffer(sizeof(cube::Creature), 0);
    cube::Creature* mockPlayer = reinterpret_cast<cube::Creature*>(creatureBuffer.data());

    mockPlayer->entity_data.specialization = 0;
    ASSERT_EQ(monk.ManaGenerationType(mockPlayer), CharacterClass::PassiveManaGain);

    mockPlayer->entity_data.specialization = 1;
    ASSERT_EQ(monk.ManaGenerationType(mockPlayer), CharacterClass::PassiveManaGain);
}

void RegisterAbilitiesAndClassesTests() {
    REGISTER_TEST(AbilitiesAndClasses, CharacterClassEnumsAndNames);
    REGISTER_TEST(AbilitiesAndClasses, MonkClassSpecialization0Abilities);
    REGISTER_TEST(AbilitiesAndClasses, MonkClassSpecialization1Abilities);
    REGISTER_TEST(AbilitiesAndClasses, MonkClassEquipmentRules);
    REGISTER_TEST(AbilitiesAndClasses, MonkClassManaGenerationStrategy);
}
