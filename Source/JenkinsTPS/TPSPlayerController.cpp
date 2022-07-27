// TPS Game For Unreal Automation


#include "TPSPlayerController.h"

void ATPSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);
    InputComponent->BindAction("ToogleGamePause", IE_Pressed, this, &ThisClass::ToogleGamePause).bExecuteWhenPaused = true;
}

void ATPSPlayerController::ToogleGamePause()
{
    SetPause(!IsPaused());
    bShowMouseCursor = IsPaused();
    IsPaused()
        ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false))
        : SetInputMode(FInputModeGameOnly());
}
