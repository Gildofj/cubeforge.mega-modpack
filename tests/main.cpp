#include "test_framework.h"

// Forward declare registration functions
void RegisterSimplexNoiseTests();
void RegisterTimerAndEventsTests();
void RegisterModSettingsParserTests();
void RegisterAbilitiesAndClassesTests();
void RegisterQuestAndInventoryTests();
void RegisterMegaModLifecycleTests();

int main() {
    RegisterSimplexNoiseTests();
    RegisterTimerAndEventsTests();
    RegisterModSettingsParserTests();
    RegisterAbilitiesAndClassesTests();
    RegisterQuestAndInventoryTests();
    RegisterMegaModLifecycleTests();

    return cubemegamod::testing::TestRunner::Instance().RunAllTests();
}
