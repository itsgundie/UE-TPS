// TPS Game For Unreal Automation

#pragma once

#include "JenkinsTPS/Tests/Utils/InputRecordingUtils.h"
#include "CoreMinimal.h"

namespace TPS
{
namespace Test
{
class JsonUtils
{
public:
    static bool WriteInputData(const FString& FileName, const FInputData& InputData);
    static bool ReadInputData(const FString& FileName, FInputData& InputData);
};
}  // namespace Test
}  // namespace TPS
