// TPS Game For Unreal Automation

#include "Settings/TPSGameUserSettings.h"
#include "Settings/TPSGameSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 QualityLevel)      \
    {                            \
        FUNC(QualityLevel);      \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "UTPSGameUserSettings"

UTPSGameUserSettings::UTPSGameUserSettings()
{
    const TArray<FSettingOption> VideoSettingsOptions = {{LOCTEXT("VideoSettingsLowQuality_Locale", "Low"), 0},
        {LOCTEXT("VideoSettingsNormalQuality_Locale", "Medium"), 1}, {LOCTEXT("VideoSettingsHighQuality_Locale", "High"), 2},
        {LOCTEXT("VideoSettingsEpicQuality_Locale", "Epic"), 3}};
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AntiAliasing_Locale", "Anti-Aliasing"));
        Setting->SetOptions(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("TextureQuality_Locale", "Textures Quality"));
        Setting->SetOptions(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("VfxQuality_Locale", "VFX Quality"));
        Setting->SetOptions(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetVisualEffectQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetVisualEffectQuality));
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("ShadowsQuality_Locale", "Shadows Quality"));
        Setting->SetOptions(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetShadingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadingQuality));
        VideoSettings.Add(Setting);
    }
    {
        auto* Setting = NewObject<UTPSGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PostProcessingQuality_Locale", "Post Processing Quality"));
        Setting->SetOptions(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
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

void UTPSGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE
