// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JENKINSTPS_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
    
public:
    void ToogleGamePause();
    
protected:
    virtual void SetupInputComponent() override;
};
