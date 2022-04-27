#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"

namespace TPS
{
namespace Test
{
template <typename Type1, typename TYpe2>
struct TestPayLoad
{
    Type1 TestValue;
    TYpe2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElement)                                                   \
    for (auto pickupType = 0; pickupType < StaticEnum<TYPE>()->NumEnums() - 1; pickupType++) \
    {                                                                                        \
        const auto EnumElement = static_cast<TYPE>(pickupType);
#define ENUM_LOOP_END }
}  // namespace Test
}  // namespace TPS
#endif
