#pragma once
#include "Misc/OutputDeviceNull.h"

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"

namespace TPS
{
namespace Test
{
DEFINE_LOG_CATEGORY_STATIC(LogTestUtils, All, All);

template <typename Type1, typename TYpe2>
struct TestPayLoad
{
    Type1 TestValue;
    TYpe2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElement)                                    \
    for (auto Index = 0; Index < StaticEnum<TYPE>()->NumEnums() - 1; ++Index) \
    {                                                                         \
        const auto EnumElement = static_cast<TYPE>(StaticEnum<TYPE>()->GetValueByIndex(Index));
#define ENUM_LOOP_END }

template <typename EnumType, typename FunctionType>
void ForEach(FunctionType&& Function)
{
    const UEnum* Enum = StaticEnum<EnumType>();
    for (auto i = 0; i < Enum->NumEnums(); ++i)
    {
        Function(static_cast<EnumType>(Enum->GetValueByIndex(i)));
    }
}

template <typename T>
T* SpawnBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* BlueprintToSpawn = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && BlueprintToSpawn) ? World->SpawnActor<T>(BlueprintToSpawn->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* SpawnBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* BlueprintToSpawn = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && BlueprintToSpawn) ? World->SpawnActorDeferred<T>(BlueprintToSpawn->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }

    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

UWorld* GetTestGameWorld();

class FTPSUntilLatentCommand : public IAutomationLatentCommand
{
public:
    FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout = 5.0f);

    virtual bool Update() override;

private:
    TFunction<void()> Callback;
    TFunction<void()> TimeoutCallback;
    float Timeout;
};

int32 GetActionBindingIndexByName(UInputComponent* InputComponent, const FString& ActionName, EInputEvent InputEvent);

int32 GetAxisBindingIndexByName(UInputComponent* InputComponent, const FString& AxisName);

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

FString GetTestDataDir();
}  // namespace Test
}  // namespace TPS
#endif
