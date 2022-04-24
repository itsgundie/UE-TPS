// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "TPSTypes.h"

#include "TPSPickupItem.generated.h"

class USphereComponent;

UCLASS()
class JENKINSTPS_API ATPSPickupItem : public AActor
{
    GENERATED_BODY()

public:
    ATPSPickupItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    
protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPickupData PickupData;
};
