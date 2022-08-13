// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GameFramework/PlayerController.h"
#include "JenkinsTPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerCOntroller.GameCanBePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerCOntroller.GameCanBeUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;


bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);

    TestTrueExpr(!PlayerController->IsPaused());
    TestTrueExpr(!PlayerController->bShowMouseCursor);

    PausePressed(PlayerController->InputComponent);
    TestTrueExpr(PlayerController->IsPaused());
    TestTrueExpr(PlayerController->bShowMouseCursor);
    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);

    PausePressed(PlayerController->InputComponent);
    TestTrueExpr(!PlayerController->IsPaused());
    TestTrueExpr(!PlayerController->bShowMouseCursor);
    return true;
}

#endif
