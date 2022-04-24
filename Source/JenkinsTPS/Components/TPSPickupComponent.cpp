// TPS Game For Unreal Automation


#include "Components/TPSPickupComponent.h"

UTPSPickupComponent::UTPSPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    
}

bool UTPSPickupComponent::TryToAddItem(const FPickupData& Data)
{
    if (Data.Score < 0)
    {
        return false;
    }
    if (!Pickup.Contains(Data.Type))
    {
        Pickup.Add(Data.Type, 0);
    }

    const auto NextScore = Pickup[Data.Type] + Data.Score;
    if (NextScore > PickupsLimits[Data.Type])
    {
        return false;
    }
    Pickup[Data.Type] = NextScore;
    return true;
}

int32 UTPSPickupComponent::GetItemCountByType(EPickupItemType Type) const
{
    return  Pickup.Contains(Type) ? Pickup[Type] : 0;
}

void UTPSPickupComponent::BeginPlay()
{
    Super::BeginPlay();

    const UEnum* PickupsEnum = StaticEnum<EPickupItemType>();
    for (int32 i = 0; i < PickupsEnum->NumEnums() - 1; i++)
    {
        const EPickupItemType EnumElement = static_cast<EPickupItemType>(i);
        const FString EnumElementName = UEnum::GetValueAsString(EnumElement);
        const bool LimitCheckCondition = PickupsLimits.Contains(EnumElement) && PickupsLimits[EnumElement] >= 0;
        checkf(LimitCheckCondition, TEXT("Limit for %s dosn't exist"), *EnumElementName);
    }
}


