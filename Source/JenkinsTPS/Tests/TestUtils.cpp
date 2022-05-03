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
    for ( const FWorldContext& Context : WorldContexts )
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World() != nullptr)
        {
            return Context.World();
        }
    }
    return nullptr;
}
}
}
#endif
