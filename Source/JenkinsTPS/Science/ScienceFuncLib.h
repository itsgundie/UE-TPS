// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"

UCLASS()
class JENKINSTPS_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Fibonacci(int32 Value);
};
