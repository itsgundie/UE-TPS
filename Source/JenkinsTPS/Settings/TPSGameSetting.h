// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "TPSGameSetting.generated.h"

USTRUCT()
struct FSettingOption
{
    GENERATED_BODY()

    FString Name;
    int32 Value;
};

UCLASS()
class UTPSGameSetting : public UObject
{
    GENERATED_BODY()
public:
    void SetName(const FString& Name);
    void SetOptions(const TArray<FSettingOption>& Options);

    FSettingOption GetCurrentOption() const;
    FString GetName() const;

private:
    FString Name;
    TArray<FSettingOption> Options;
};
