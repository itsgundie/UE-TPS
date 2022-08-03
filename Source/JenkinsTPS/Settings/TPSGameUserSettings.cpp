// TPS Game For Unreal Automation


#include "Settings/TPSGameUserSettings.h"
#include "Settings/TPSGameSetting.h"

UTPSGameUserSettings::UTPSGameUserSettings()
{
    const TArray<FSettingOption> VFXOptions =
    {
        {"Low", 0},
        {"Medium", 1},
        {"High", 2},
        {"Epic", 3}
    };
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName("Anti-Aliasing");
        Setting->SetOptions(VFXOptions);
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName("Textures Quality");
        Setting->SetOptions(VFXOptions);
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName("Global Illumination");
        Setting->SetOptions(VFXOptions);
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName("Shadows Quality");
        Setting->SetOptions(VFXOptions);
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName("Post Processing Quality");
        Setting->SetOptions(VFXOptions);
        VideoSettings.Add(Setting);
    }
}

UTPSGameUserSettings* UTPSGameUserSettings::Get()
{
    return GEngine ? Cast<UTPSGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UTPSGameSetting*>& UTPSGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}
