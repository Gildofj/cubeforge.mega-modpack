#include "../test_framework.h"
#include "inventory/Timer.h"
#include "abilities/Event.h"
#include "abilities/EventList.h"

// Mock Event for testing without engine game state dependencies
class MockGameEvent : public cube::Event {
public:
    int updateCalls = 0;
    MockGameEvent(cube::EventType type) {
        this->eventType = type;
    }

    void Update() override {
        cube::Event::Update();
        updateCalls++;
    }
};

TEST_FUNC(TimerAndEvents, TimerTriggerAndCooldown) {
    // 5-second interval timer starting at time = 1000
    time_t baseTime = 1000;
    cube::Timer timer(5.0, baseTime);

    // 4 seconds later -> Should NOT trigger
    ASSERT_FALSE(timer.IsTriggered(baseTime + 4, true));

    // 5 seconds later -> SHOULD trigger and reset baseTime to 1005
    ASSERT_TRUE(timer.IsTriggered(baseTime + 5, true));

    // Immediately at 1005 -> Should NOT trigger because it was reset
    ASSERT_FALSE(timer.IsTriggered(baseTime + 5, true));

    // 3 seconds after reset (time = 1008) -> Should NOT trigger
    ASSERT_FALSE(timer.IsTriggered(baseTime + 8, true));

    // 6 seconds after reset (time = 1011) with reset = false
    ASSERT_TRUE(timer.IsTriggered(baseTime + 11, false));

    // Calling again at time = 1011 should STILL trigger because reset was false
    ASSERT_TRUE(timer.IsTriggered(baseTime + 11, false));

    // Now trigger with reset = true
    ASSERT_TRUE(timer.IsTriggered(baseTime + 11, true));

    // Calling at time = 1012 should now return false
    ASSERT_FALSE(timer.IsTriggered(baseTime + 12, true));
}

TEST_FUNC(TimerAndEvents, EventListAddAndFind) {
    cube::EventList list;
    ASSERT_EQ(list.events.size(), 0);

    auto e1 = new MockGameEvent(cube::EventType::AddGold);
    auto e2 = new MockGameEvent(cube::EventType::Diving);

    list.Add(e1);
    list.Add(e2);
    list.Add(nullptr); // Safe handling of nullptr

    ASSERT_EQ(list.events.size(), 2);

    cube::Event* foundGold = list.Find(cube::EventType::AddGold);
    ASSERT_TRUE(foundGold != nullptr);
    ASSERT_EQ(foundGold->eventType, cube::EventType::AddGold);

    cube::Event* foundDiving = list.Find(cube::EventType::Diving);
    ASSERT_TRUE(foundDiving != nullptr);
    ASSERT_EQ(foundDiving->eventType, cube::EventType::Diving);

    cube::Event* foundNone = list.Find(cube::EventType::None);
    ASSERT_TRUE(foundNone == nullptr);
}

TEST_FUNC(TimerAndEvents, EventListRemoveAndClear) {
    cube::EventList list;
    list.Add(new MockGameEvent(cube::EventType::AddGold));
    list.Add(new MockGameEvent(cube::EventType::Diving));

    ASSERT_EQ(list.events.size(), 2);

    list.Remove(cube::EventType::AddGold);
    ASSERT_EQ(list.events.size(), 1);
    ASSERT_TRUE(list.Find(cube::EventType::AddGold) == nullptr);
    ASSERT_TRUE(list.Find(cube::EventType::Diving) != nullptr);

    // Removing non-existent event should be safe
    list.Remove(cube::EventType::None);
    ASSERT_EQ(list.events.size(), 1);

    list.Clear();
    ASSERT_EQ(list.events.size(), 0);
    ASSERT_TRUE(list.Find(cube::EventType::Diving) == nullptr);
}

TEST_FUNC(TimerAndEvents, EventListMoveSemantics) {
    cube::EventList list1;
    list1.Add(new MockGameEvent(cube::EventType::AddGold));
    list1.Add(new MockGameEvent(cube::EventType::Diving));

    cube::EventList list2 = std::move(list1);
    ASSERT_EQ(list2.events.size(), 2);
    ASSERT_TRUE(list2.Find(cube::EventType::AddGold) != nullptr);

    cube::EventList list3;
    list3.Add(new MockGameEvent(cube::EventType::None));
    list3 = std::move(list2);
    ASSERT_EQ(list3.events.size(), 2);
    ASSERT_TRUE(list3.Find(cube::EventType::AddGold) != nullptr);
}

void RegisterTimerAndEventsTests() {
    REGISTER_TEST(TimerAndEvents, TimerTriggerAndCooldown);
    REGISTER_TEST(TimerAndEvents, EventListAddAndFind);
    REGISTER_TEST(TimerAndEvents, EventListRemoveAndClear);
    REGISTER_TEST(TimerAndEvents, EventListMoveSemantics);
}
