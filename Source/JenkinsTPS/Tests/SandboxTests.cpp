// TPS Game For Unreal Automation


#include "SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::MediumPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] function testing");
    TestTrue("2 different positive numbers", FMath::Max(4, 51) == 51);
    TestEqual("2 equal positive numbers", FMath::Max(451, 451), 451);
    TestTrueExpr(FMath::Max(0, 451) == 451);
    TestTrue("2 zeroes", FMath::Max(0, 0) == 0);
    TestTrue("Negative and Positive numbers", FMath::Max(451, -451) == 451);
    TestTrue("Negative and zero", FMath::Max(-451, 0) == 0);
    TestTrue("2 different negative numbers", FMath::Max(-1, -45) == -1);
    TestTrue("2 equal negative numbers", FMath::Max(-451, -451) == -451);
    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Square Root function testing");
    TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
    TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);
    return true;
}