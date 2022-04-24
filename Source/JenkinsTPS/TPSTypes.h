#pragma once

#include "CoreMinimal.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType: uint8
{
    SPHERE = 0 UMETA(DisplayName = "Testing Sphere"),
    CUBE UMETA(DisplayName = "Testing Cube"),
    CYLINDER,
    CONE
};

USTRUCT(BlueprintType)
struct FPickupData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPickupItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;
};

