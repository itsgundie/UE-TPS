// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class JENKINSTPS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CloseMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnCloseMenu();
};
