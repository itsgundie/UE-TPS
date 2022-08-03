// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Settings/TPSGameSetting.h"
#include "SettingOptionWidget.generated.h"

class UTextBlock;
class UTPSGameSetting;

UCLASS()
class JENKINSTPS_API USettingOptionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingDisplayName;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingCurrentValue;

    virtual void NativeOnInitialized() override;

private:
    TWeakObjectPtr<UTPSGameSetting> Setting;

    void Init(UTPSGameSetting* Setting);

    void UpdateTexts();

    friend class UVideoSettingsWidget;
};
