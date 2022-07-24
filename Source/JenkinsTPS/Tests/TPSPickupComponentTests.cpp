// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSPickupComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Components/TPSPickupComponent.h"
#include "TPSTypes.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCanBeCreated, "TPSGame.Components.Pickup.ComponentCanBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupScoreIsZeroDyDefault, "TPSGame.Components.Pickup.PickupScoreIsZeroDyDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreCantBeAdded, "TPSGame.Components.Pickup.NegativeScoreCantBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "TPSGame.Components.Pickup.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded, "TPSGame.Components.Pickup.ScoreMoreThanLimitCantBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
class UTPSPickupComponentTestable : public UTPSPickupComponent
{
public:
    void SetLimits(const TMap<EPickupItemType, int32>& Limits) { PickupsLimits = Limits; }
};

TMap<EPickupItemType, int32> InitPickupLimits(UTPSPickupComponentTestable* PickupComponent, int32 LimitValue)
{
    TMap<EPickupItemType, int32> PickupLimits;
    ENUM_LOOP_START(EPickupItemType, EEnumElement);
    PickupLimits.Add(EEnumElement, LimitValue);
    ENUM_LOOP_END;
    PickupComponent->SetLimits(PickupLimits);
    return PickupLimits;
}
}  // namespace

const int32 ScoreLimit = 100;

bool FComponentCanBeCreated::RunTest(const FString& Parameters)
{
    const UTPSPickupComponent* PickupComponent = NewObject<UTPSPickupComponent>();
    if (!TestNotNull("Pickup Component Created", PickupComponent))
    {
        return false;
    }
    return true;
}

bool FPickupScoreIsZeroDyDefault::RunTest(const FString& Parameters)
{
    const UTPSPickupComponent* PickupComponent = NewObject<UTPSPickupComponent>();
    if (!TestNotNull("Pickup Component Created", PickupComponent))
    {
        return false;
    }
    ForEach<EPickupItemType>([&](EPickupItemType EEnumElement)
        {TestTrueExpr(PickupComponent->GetItemCountByType(EEnumElement) == 0);});
    return true;
}

bool FNegativeScoreCantBeAdded::RunTest(const FString& Parameters)
{
    const int InvalidScoreAmount = -1;
    const int ValidScoreAmount = 1;
    UTPSPickupComponentTestable* PickupComponent = NewObject<UTPSPickupComponentTestable>();
    if (!TestNotNull("Pickup Component Created", PickupComponent))
    {
        return false;
    }
    InitPickupLimits(PickupComponent, ScoreLimit);

    ENUM_LOOP_START(EPickupItemType, EEnumElement)
    TestTrueExpr(PickupComponent->TryToAddItem({EEnumElement, ValidScoreAmount}));
    TestTrueExpr(PickupComponent->GetItemCountByType(EEnumElement) == ValidScoreAmount);
    TestTrueExpr(!PickupComponent->TryToAddItem({EEnumElement, InvalidScoreAmount}));
    TestTrueExpr(PickupComponent->GetItemCountByType(EEnumElement) == ValidScoreAmount);
    ENUM_LOOP_END
    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPSPickupComponentTestable* PickupComponent = NewObject<UTPSPickupComponentTestable>();
    if (!TestNotNull("Pickup Component Created", PickupComponent))
    {
        return false;
    }
    const auto PickupLimits = InitPickupLimits(PickupComponent, ScoreLimit);

    ENUM_LOOP_START(EPickupItemType, EEnumElement)
    for (int32 i = 0; i < PickupLimits[EEnumElement]; i++)
    {
        TestTrueExpr(PickupComponent->TryToAddItem({EEnumElement, 1}));
        TestTrueExpr(PickupComponent->GetItemCountByType(EEnumElement) == i + 1);
    }
    ENUM_LOOP_END
    return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
    UTPSPickupComponentTestable* PickupComponent = NewObject<UTPSPickupComponentTestable>();
    if (!TestNotNull("Pickup Component Created", PickupComponent))
    {
        return false;
    }
    InitPickupLimits(PickupComponent, ScoreLimit);
    ENUM_LOOP_START(EPickupItemType, EEnumElement)
    TestTrueExpr(PickupComponent->TryToAddItem({EEnumElement, ScoreLimit}));
    TestTrueExpr(!PickupComponent->TryToAddItem({EEnumElement, 1}));
    TestTrueExpr(PickupComponent->GetItemCountByType(EEnumElement) == ScoreLimit);
    ENUM_LOOP_END
    return true;
}

#endif
