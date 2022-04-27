// TPS Game For Unreal Automation

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::MediumPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::MediumPriority);

using namespace TPS::Test;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] function testing");

    typedef TArray<TestPayLoad<TInterval<int32>, int32>> MaxIntTestPayload;
    // clang-format off
    const MaxIntTestPayload TestData
    {
        {{4, 51},51},
        {{451, 451},451},
        {{0, 451},451},
        {{0, 0},0},
        {{451, -451},451},
        {{-451, 0},0},
        {{-1, -45},-1},
        {{-451, -451},-451},
    };
    // clang-format on

    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
    }
    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    typedef TArray<TestPayLoad<float, float>> SqrtTestPayload;
    // clang-format off
    const SqrtTestPayload TestData
    {
        {4.0f, 2.0f},
        {3.0f, 1.7f, 0.1f},
        {3.0f, 1.73f, 0.01f},
        {3.0f, 1.73205f, 1.e-5f},
    };
    // clang-format on
    for (const auto Data : TestData)
    {
        const bool isEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);
        TestTrueExpr(isEqual);
    }

    AddInfo("Square Root function testing");
    TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
    TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);
    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    typedef float Degrees;
    typedef TArray<TestPayLoad<Degrees, float>> SinTestPayload;
    // clang-format off
    const SinTestPayload TestData
    {
                {Degrees{0.00f},0.0f},
                {Degrees{30.0f},0.5f},
                {Degrees{45.0f},0.707f},
                {Degrees{60.0f},0.866f},
                {Degrees{90.0f},1.0f},
    };
    // clang-format on

    for (const auto Data : TestData)
    {
        const float Rad = FMath::DegreesToRadians(Data.TestValue);
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Rad), Data.ExpectedValue, 0.001f));
    }
    return true;
}

#endif