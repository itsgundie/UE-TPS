// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "InputRecordingUtils.generated.h"

USTRUCT()
struct FAxisData
{
    GENERATED_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    float Value;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FAxisData> AxisValues;

    // TODO ActionValues
    
    UPROPERTY()
    float WorldTime{0.0f};
};

USTRUCT()
struct FInputData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FBindingsData> Bindings;

    UPROPERTY()
    FTransform InitialTransform;
};
