// Copyright Epic Games, Inc. All Rights Reserved.

#include "JenkinsTPSGameMode.h"
#include "JenkinsTPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJenkinsTPSGameMode::AJenkinsTPSGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
