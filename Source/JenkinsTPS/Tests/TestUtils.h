#pragma once

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
}
}
