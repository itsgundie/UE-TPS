// TPS Game For Unreal Automation

#include "Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScience, All, All);

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
    if (Value < 0)
    {
        UE_LOG(LogScience, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci((Value - 2));
};
