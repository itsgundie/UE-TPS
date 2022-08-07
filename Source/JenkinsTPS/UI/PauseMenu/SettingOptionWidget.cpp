// TPS Game For Unreal Automation


#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Settings/TPSGameSetting.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    check(SettingDisplayName);
    check(SettingCurrentValue);
    check(NextSettingButton);
    check(PrevSettingButton);

    NextSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnNextSetting);
    PrevSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnPreviousSetting);
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
        SettingDisplayName->SetText(Setting->GetName());
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnNextSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyNextOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnPreviousSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyPreviousOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}
