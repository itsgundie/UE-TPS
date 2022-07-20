// My game copyright

#include "GameFramework/CharacterMovementComponent.h"
#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Engine/World.h"
#include "JenkinsTPSCharacter.h"
#include "TPSTypes.h"
#include "Tests/AutomationCommon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterStatic, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthDecreasedByDamage, "TPSGame.Character.HealthDecreasedByDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "TPSGame.Character.CharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "TPSGame.Character.AutoHealShouldRestoreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait, "TPSGame.LatentCommand.SimpleWait",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog, "TPSGame.LatentCommand.SimpleLog",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "TPSGame.LatentCommand.OpenCloseMap",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

using namespace TPS::Test;

namespace
{
constexpr char* TestCharacterBPName = "Blueprint'/Game/Tests/BP_Test_TPSCharacter.BP_Test_TPSCharacter'";
} // namespace

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

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, AJenkinsTPSCharacter*, Character, float, LifeSpan);

bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterStatic, Error, TEXT("Character wasn't destroyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;
    const FTransform InitialTransform{FVector{0.0f, -120.0f, 200.0f}};
    AJenkinsTPSCharacter* TestCharacter = SpawnBlueprintDeferred<AJenkinsTPSCharacter>(World, TestCharacterBPName, InitialTransform);
    if (!TestNotNull("Test Character item exists", TestCharacter)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 1.5f;
    CallFuncByNameWithParams(TestCharacter, "SetHealthData", {HealthData.ToString()});
    TestCharacter->FinishSpawning(InitialTransform);
    TestEqual("Health is Full", TestCharacter->GetHealthPercent(), 1.0f);

    const auto DamageAmountForKill = HealthData.MaxHealth;
    TestCharacter->TakeDamage(DamageAmountForKill, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health equal zero, character is dead", TestCharacter->GetHealthPercent(), 0.0f);
    TestTrueExpr(TestCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);
    ENUM_LOOP_START(ECollisionChannel, EElement)
        if (EElement != ECC_OverlapAll_Deprecated)
        {
            TestTrueExpr(TestCharacter->GetCapsuleComponent()->GetCollisionResponseToChannel(EElement) == ECollisionResponse::ECR_Ignore);
        }
    ENUM_LOOP_END
    TestTrueExpr(TestCharacter->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(TestCharacter->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(TestCharacter->GetLifeSpan(), HealthData.LifeSpan));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([TestCharacter]()
        {
        if (IsValid(TestCharacter))
        {
        UE_LOG(LogTPSCharacterStatic, Error, TEXT("Character wasn't destroyed"));
        }
        }, HealthData.LifeSpan));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, AJenkinsTPSCharacter*, Character, float, HealingDuration);

bool FAutoHealCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterStatic, Error, TEXT("Character's health wasn't recovered to full"));
        }
        return true;
    }
    return false;
}

bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -120.0f, 200.0f}};
    AJenkinsTPSCharacter* TestCharacter = SpawnBlueprintDeferred<AJenkinsTPSCharacter>(World, TestCharacterBPName, InitialTransform);
    if (!TestNotNull("Test Character item exists", TestCharacter)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 300.0f;
    HealthData.HealRatio = 10.0f;
    HealthData.HealRate = 0.5f;
    CallFuncByNameWithParams(TestCharacter, "SetHealthData", {HealthData.ToString()});
    TestCharacter->FinishSpawning(InitialTransform);

    const float DamageAmount = 100.0f;
    TestEqual("Health is Full", TestCharacter->GetHealthPercent(), 1.0f);
    TestCharacter->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", TestCharacter->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - TestCharacter->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealRatio;
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([TestCharacter]()
        {
        if (!FMath::IsNearlyEqual(TestCharacter->GetHealthPercent(), 1.0f))
        {
        UE_LOG(LogTPSCharacterStatic, Error, TEXT("Character's health wasn't recovered to full"));
        }
        }, HealingDuration));
    return true;
}


bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.5f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPSLatentLogCommand, FString, LogMessage);

bool FTPSLatentLogCommand::Update()
{
    UE_LOG(LogTPSCharacterStatic, Display, TEXT("%s"), *LogMessage);
    return true;
}


bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogTPSCharacterStatic, Warning, TEXT("Before Latent Commands"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommand("Latent Log #1"));
    UE_LOG(LogTPSCharacterStatic, Warning, TEXT("Between Latent Commands"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommand("Latent Log #2"));
    UE_LOG(LogTPSCharacterStatic, Warning, TEXT("After Latent Commands"));
    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(4.2f));
    return true;
}


#endif
