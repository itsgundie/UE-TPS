// TPS Game For Unreal Automation


#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/ScienceFuncLibTests.h"
#include "TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/ScienceFuncLib.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "TPSGame.Science.Fibonacci.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "TPSGame.Science.Fibonacci.Stress",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "TPSGame.Science.Fibonacci.LogHasErrors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci sequence simple calculation with predefined pairs of positive numbers");

    const TArray<TPS::Test::TestPayLoad<int32, int32>> TestData
    {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 3},
        {5, 5},
    };
    for (const auto Data : TestData)
    {
        const FString InfoString = FString::Printf(TEXT("Test Value: %i, Expected Value: %i"), Data.TestValue, Data.ExpectedValue);
        TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }
    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci sequence calculation stress testing for 40 numbers staring form 2");

    for (int32 i = 2; i < 42; i++)
    {
        TestTrueExpr(UScienceFuncLib::Fibonacci(i) == //
            UScienceFuncLib::Fibonacci(i - 1) + UScienceFuncLib::Fibonacci(i - 2));
    }
    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci sequence calculation for negative number produces error");
    AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
    UScienceFuncLib::Fibonacci(-13);
    return true;
}


#endif
