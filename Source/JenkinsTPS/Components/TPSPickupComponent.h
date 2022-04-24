// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSTypes.h"
#include "TPSPickupComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JENKINSTPS_API UTPSPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSPickupComponent();

    bool TryToAddItem(const FPickupData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetItemCountByType(EPickupItemType Type) const;

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EPickupItemType, int32> PickupsLimits;

private:
    TMap<EPickupItemType, int32> Pickup;
};
