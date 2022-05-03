#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"

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

template<typename T>
T* SpawnBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* BlueprintToSpawn = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && BlueprintToSpawn) ? World->SpawnActor<T>(BlueprintToSpawn->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

UWorld* GetTestGameWorld();

}  // namespace Test
}  // namespace TPS
#endif
