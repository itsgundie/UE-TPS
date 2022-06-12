#pragma once

#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"

namespace TPS
{
namespace Test
{
UWorld* TPS::Test::GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World() != nullptr)
        {
            return Context.World();
        }
    }
    return nullptr;
}

FTPSUntilLatentCommand::FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout):
Callback(MoveTemp(InCallback)),
TimeoutCallback(MoveTemp(InTimeoutCallback)),
Timeout(InTimeout)
{
    
}

bool FTPSUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }
    Callback();
    return false;
}

int32 GetActionBindingIndexByName(UInputComponent* InputComponent, const FString& ActionName, EInputEvent InputEvent)
{
    if (!InputComponent) return INDEX_NONE;
    for (auto i = 0; i < InputComponent->GetNumActionBindings(); ++i)
    {
        const FInputActionBinding Action = InputComponent->GetActionBinding(i);
        if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

int32 GetAxisBindingIndexByName(UInputComponent* InputComponent, const FString& AxisName)
{
    if (!InputComponent) return INDEX_NONE;
    const int32 AxisIndex = InputComponent->AxisBindings.IndexOfByPredicate([=](const FInputAxisBinding& AxisBinding)
        {return AxisBinding.AxisName.ToString().Equals(AxisName);});
    return AxisIndex;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object) return;

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }
    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}
}  // namespace Test
}  // namespace TPS
#endif
