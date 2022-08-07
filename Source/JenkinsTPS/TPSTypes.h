#pragma once

#include "CoreMinimal.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "Testing Sphere"),
    CUBE UMETA(DisplayName = "Testing Cube"),
    CYLINDER UMETA(DisplayName = "Testing Cylinder"),
    CONE UMETA(DisplayName = "Testing Cone")
};

USTRUCT(BlueprintType)
struct FPickupData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPickupItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;

    FString ToString() const { return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
    float MaxHealth{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float HealRatio{5.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float HealRate{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float LifeSpan{5.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "-5.0", Units = "s"))
    float HealDelay{-1.0f};

    FString ToString() const
    {
        return FString::Printf(
            TEXT("(MaxHealth=%f,HealRatio=%f,HealRate=%f,LifeSpan=%f,HealDelay=%f)"), MaxHealth, HealRatio, HealRate, LifeSpan, HealDelay);
    }
};
