// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JenkinsTPSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, isPaused);

UCLASS(minimalapi)
class AJenkinsTPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AJenkinsTPSGameMode();
    virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate) override;
    virtual bool ClearPause() override;

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;
};
