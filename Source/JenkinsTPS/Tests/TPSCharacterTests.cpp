// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Engine/World.h"
#include "JenkinsTPSCharacter.h"
#include "TPSTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthDecreasedByDamage, "TPSGame.Character.HealthDecreasedByDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
constexpr char* TestCharacterBPName = "Blueprint'/Game/Tests/BP_Test_TPSCharacter.BP_Test_TPSCharacter'";
}  // namespace

bool FHealthDecreasedByDamage::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -120.0f, 200.0f}};
    AJenkinsTPSCharacter* TestCharacter = SpawnBlueprintDeferred<AJenkinsTPSCharacter>(World, TestCharacterBPName, InitialTransform);
    if (!TestNotNull("Test Character item exists", TestCharacter)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    CallFuncByNameWithParams(TestCharacter, "SetHealthData", {HealthData.ToString()});
    TestCharacter->FinishSpawning(InitialTransform);

    const float DamageAmount = 10.0f;
    TestEqual("Health is Full", TestCharacter->GetHealthPercent(), 1.0f);
    TestCharacter->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", TestCharacter->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);
    
    return true;
}

#endif
