// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Items/TPSPickupItem.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCanBePickedWhileJumping, "TPSGame.Gameplay.PickupCanBePickedWhileJumping",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTooHighPickupCantBePickedWhileJumping, "TPSGame.Gameplay.TooHighPickupCantBePickedWhileJumping",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllPickupsCanBeTakenWhileMoving, "TPSGame.Gameplay.AllPickupsCanBeTakenWhileMoving",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);
bool FJumpLatentCommand::Update()
{
    if (!Character) return true;
    const int32 ActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
    if (ActionIndex != INDEX_NONE)
    {
        const auto JumpActionBind = Character->InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::SpaceBar);
    }
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
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
    {
        TArray<AActor*> PickupItems;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
        TestTrueExpr(PickupItems.Num() == 0);
    }, 2.0f));
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
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
    {
        TArray<AActor*> PickupItems;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
        TestTrueExpr(PickupItems.Num() == 1);
    }, 2.0f));
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
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand([=]()
    {
        Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f);
    }, [](){}, 3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand([=]()
        {
        Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f);
        }, [](){}, 2.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=]()
        {
        TArray<AActor*> PickupItems;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSPickupItem::StaticClass(), PickupItems);
        TestTrueExpr(PickupItems.Num() == 6);
        return true;
        }));
    return true;
}

#endif
