// TPS Game For Unreal Automation

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JenkinsTPS/Tests/Utils/InputRecordingUtils.h"
#include "TPSInputRecordingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JENKINSTPS_API UTPSInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTPSInputRecordingComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FInputData InputData;

    FBindingsData MakeBindingsData() const;
    FString GenerateFileName() const;
};
