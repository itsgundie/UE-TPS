// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Items/TPSPickupItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Tests/Utils/JsonUtils.h"
#include "Tests/Utils/InputRecordingUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCanBePickedWhileJumping, "TPSGame.Gameplay.PickupCanBePickedWhileJumping",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTooHighPickupCantBePickedWhileJumping, "TPSGame.Gameplay.TooHighPickupCantBePickedWhileJumping",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllPickupsCanBeTakenWhileMoving, "TPSGame.Gameplay.AllPickupsCanBeTakenWhileMoving",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllPickupsCanBeTakenWhileRecordedMoving, "TPSGame.Gameplay.AllPickupsCanBeTakenWhileRecordedMoving",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllMapsShouldBeLoaded, "TPSGame.Gameplay.AllMapsShouldBeLoaded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
    return true;
}

bool FPickupCanBePickedWhileJumping::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/PickupTestLevel_1");
    UWorld* World = GetTestGameWorld();

    if (!TestNotNull("Game World Exists", World)) return false;
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character Exists", Character)) return false;
    TArray<AActor*> PickupItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
    if (!TestEqual("Exists Only One Pickup", PickupItems.Num(), 1)) return false;
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
            TestTrueExpr(PickupItems.Num() == 0);
        },
        2.0f));
    return true;
}

bool FTooHighPickupCantBePickedWhileJumping::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/PickupTestLevel_2");
    UWorld* World = GetTestGameWorld();

    if (!TestNotNull("Game World Exists", World)) return false;
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character Exists", Character)) return false;
    TArray<AActor*> PickupItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
    if (!TestEqual("Exists Only One Pickup", PickupItems.Num(), 1)) return false;
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
            TestTrueExpr(PickupItems.Num() == 11);
        },
        2.0f));
    return true;
}

bool FAllPickupsCanBeTakenWhileMoving::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/PickupTestLevel_3");
    UWorld* World = GetTestGameWorld();

    if (!TestNotNull("Game World Exists", World)) return false;
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character Exists", Character)) return false;
    TArray<AActor*> PickupItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
    TestTrueExpr(PickupItems.Num() == 3);
    const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveForward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);
    const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveRight");
    TestTrueExpr(MoveRightIndex != INDEX_NONE);
    Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); }, []() {}, 3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(
        [=]() { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); }, []() {}, 2.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
            TestTrueExpr(PickupItems.Num() == 6);
            return true;
        }));
    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
        : World(InWorld), InputComponent(InInputComponent), BindingsData(InBindingsData)
    {
    }

    virtual bool Update() override
    {
        if (!World || !InputComponent) return true;
        if (WorldsStartTime == 0.0f)
        {
            WorldsStartTime = World->TimeSeconds;
        }
        while (World->TimeSeconds - WorldsStartTime >= BindingsData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->AxisBindings.Num(); i++)
            {
                const float AxisValue = BindingsData[Index].AxisValues[i].Value;
                InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
            }
            if (++Index >= BindingsData.Num()) return true;
        }
        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> BindingsData;
    int32 Index{0};
    float WorldsStartTime{0.0f};
};

void FAllPickupsCanBeTakenWhileRecordedMoving::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData = {{"MainMap", "/Game/ThirdPersonCPP/Maps/MainMap.MainMap", "CharacterTestInputMainMap.json"},
        {"CustomMap", "/Game/ThirdPersonCPP/Maps/CustomMap.CustomMap", "CharactersTestInputCustomMap.json"}};

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

bool FAllPickupsCanBeTakenWhileRecordedMoving::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map name and JSON params should exists", ParsedParams.Num() == 2)) return false;

    const auto Level = LevelScope(ParsedParams[0]);
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character Exists", Character)) return false;
    TArray<AActor*> PickupItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
    TestTrueExpr(PickupItems.Num() == 4);

    const FString FileName = GetTestDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input Data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player Controller Exists", PlayerController)) return false;
    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> PickupItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
            TestTrueExpr(PickupItems.Num() == 4);
            return true;
        }));
    return true;
}

void FAllMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    const TArray<TTuple<FString, FString>> TestData = {MakeTuple("MainMap", "/Game/ThirdPersonCPP/Maps/MainMap.MainMap"),
        MakeTuple("CustomMap", "/Game/ThirdPersonCPP/Maps/CustomMap.CustomMap")};
    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

bool FAllMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(4.2f));
    return true;
}

#endif
