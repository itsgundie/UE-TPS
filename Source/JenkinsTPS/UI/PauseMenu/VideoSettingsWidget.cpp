// TPS Game For Unreal Automation


#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Settings/TPSGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    const auto* UserSettings = UTPSGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("TPSGameUserSettings is nullptr"));
        return;
    }

    const auto VideoSettings = UserSettings->GetVideoSettings();
    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting: VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }
}
