// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TPSGameUserSettings.generated.h"

class UTPSGameSetting;

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

UCLASS()
class JENKINSTPS_API UTPSGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
    UTPSGameUserSettings();
    static  UTPSGameUserSettings* Get();

    const TArray<UTPSGameSetting*>& GetVideoSettings() const;

    void RunBenchmark();
    FOnSettingsUpdatedDelegate OnVideoSettingsUpdated;

private:
    UPROPERTY()
    TArray<UTPSGameSetting*> VideoSettings;
};
