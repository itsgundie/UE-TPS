// TPS Game For Unreal Automation


#include "Settings/TPSGameSetting.h"

void UTPSGameSetting::SetName(const FString& InName)
{
    Name = InName;
}

void UTPSGameSetting::SetOptions(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UTPSGameSetting::GetCurrentOption() const
{
    return Options[0];
}

FString UTPSGameSetting::GetName() const
{
    return  Name;
}
