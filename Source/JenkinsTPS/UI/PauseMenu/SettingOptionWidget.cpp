// TPS Game For Unreal Automation


#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Settings/TPSGameSetting.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    check(SettingDisplayName);
    check(SettingCurrentValue);
}

void USettingOptionWidget::Init(UTPSGameSetting* InSetting)
{
    Setting = MakeWeakObjectPtr(InSetting);
    check(Setting.IsValid());

    UpdateTexts();
}

void USettingOptionWidget::UpdateTexts()
{
    if (Setting.IsValid())
    {
        SettingDisplayName->SetText(FText::FromString(Setting->GetName()));
        SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
    }
}
