#include "test_framework.h"
#include "cwsdk.h"

// Stubs for hooks in testing environment
extern "C" {
    void* ASMOnCreatureDeath_jmpback = nullptr;
    void* ASMOnTalkToCreature_jmpback = nullptr;
    void* ASMOnTalkToCreature_jmpback2 = nullptr;
    void* ASMOnTalkToCreature_bail = nullptr;

    void OnCreatureDeath(cube::Creature* creature, cube::Creature* attacker)
    {
        (void)creature;
        (void)attacker;
    }

    int OnTalkToCreature(cube::Game* game, cube::Creature* creature)
    {
        (void)game;
        (void)creature;
        return 0;
    }
}

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

    return cubeforge::testing::TestRunner::Instance().RunAllTests();
}
