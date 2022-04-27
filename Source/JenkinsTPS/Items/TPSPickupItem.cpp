// TPS Game For Unreal Automation

#include "Items/TPSPickupItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "TPSTypes.h"
#include "JenkinsTPS/Components/TPSPickupComponent.h"

ATPSPickupItem::ATPSPickupItem()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(20.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(CollisionComponent);
}

void ATPSPickupItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto PickupComponent = Pawn->FindComponentByClass<UTPSPickupComponent>())
        {
            if (PickupComponent->TryToAddItem(PickupData))
            {
                Destroy();
            }
        }
    }
}
