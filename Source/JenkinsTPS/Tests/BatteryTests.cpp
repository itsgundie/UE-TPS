// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "TPSGame.Items.Battery", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
    using namespace TPS;

    AddInfo("Battery with default constructor");
    const Battery BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(BatteryDefault.ToString().Equals("100%"));

    AddInfo("Battery with custom constructor");
    const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentString)
    {
        const Battery BatteryObject{Percent};
        TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
        TestTrueExpr(BatteryObject.GetColor() == Color);
        TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
    };

    BatteryTestFunc(1.0f, FColor::Green, "100%");
    BatteryTestFunc(1.01f, FColor::Green, "100%");
    BatteryTestFunc(1234.56f, FColor::Green, "100%");
    BatteryTestFunc(0.81f, FColor::Green, "81%");
    BatteryTestFunc(0.8f, FColor::Yellow, "80%");
    BatteryTestFunc(0.42f, FColor::Yellow, "42%");
    BatteryTestFunc(0.31f, FColor::Yellow, "31%");
    BatteryTestFunc(0.30f, FColor::Red, "30%");
    BatteryTestFunc(0.01f, FColor::Red, "1%");
    BatteryTestFunc(0.0f, FColor::Red, "0%");
    BatteryTestFunc(-0.01f, FColor::Red, "0%");
    BatteryTestFunc(-65.4321f, FColor::Red, "0%");

    AddInfo("Battery Charge and Discharge tests");
    Battery BatteryObject{0.75f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.75f, 0.009f));
    BatteryObject.Discharge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.65f, 0.009f));
    BatteryObject.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.75f, 0.009f));

    for (int32 i = 0; i < 5; i++)
    {
        BatteryObject.Charge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));

    for (int32 i = 0; i < 12; i++)
    {
        BatteryObject.Discharge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

    AddInfo("Batteries Values Comparison");
    const Battery BatteryHigh{0.99f};
    const Battery BatteryHighSameAmount{0.99f};
    const Battery BatteryMid{0.5f};
    const Battery BatteryMidSameAmount{0.5f};
    const Battery BatteryLow{0.13f};
    const Battery BatteryLowSameAMount{0.13f};

    TestTrueExpr(FMath::Max(BatteryHigh, BatteryMid) == BatteryHigh);
    TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);
    TestTrueExpr(FMath::Max(BatteryMid, BatteryLow) == BatteryMid);
    TestTrueExpr(FMath::Max(BatteryHighSameAmount, BatteryHigh) == BatteryHigh);
    TestTrueExpr(FMath::Max(BatteryMidSameAmount, BatteryMid) == BatteryMid);
    TestTrueExpr(FMath::Max(BatteryLow, BatteryLowSameAMount) == BatteryLowSameAMount);

    AddInfo("Battery object are same in memory");
    TestNotSame("Batteries are not same object", BatteryMid, BatteryHigh);
    TestNotSame("Batteries are not same object", BatteryLow, BatteryLowSameAMount);
    const Battery& BatteryLowDuplicate = BatteryLow;
    TestSame("Batteries are the same object", BatteryLow, BatteryLowDuplicate);
    return true;
}

#endif
