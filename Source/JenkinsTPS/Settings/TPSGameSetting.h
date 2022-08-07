// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "TPSGameSetting.generated.h"

struct FSettingOption
{
    FText Name;
    int32 Value;
};

UCLASS()
class UTPSGameSetting : public UObject
{
    GENERATED_BODY()
public:
    void SetName(const FText& Name);
    void SetOptions(const TArray<FSettingOption>& Options);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;
    void AddGetter(TFunction<int32()> Func);
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyNextOption();
    void ApplyPreviousOption();

private:
    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    int32 GetCurrentValue() const;
    void SerCurrentValue(int32 Value);
    int32 GetCurrentIndex() const;
};
