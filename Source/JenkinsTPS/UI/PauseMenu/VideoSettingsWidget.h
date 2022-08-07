// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "SettingOptionWidget.h"
#include "Blueprint/UserWidget.h"
#include "VideoSettingsWidget.generated.h"

class UVerticalBox;
class USettingOptionWidget;
class UButton;

UCLASS()
class JENKINSTPS_API UVideoSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VideoSettingsContainer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<USettingOptionWidget> SettingOptionWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UButton* RunBenchmarkButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBenchmark();

    void OnVideoSettingsUpdated();
};
