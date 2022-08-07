// TPS Game For Unreal Automation

#include "Tests/Components/TPSInputRecordingComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "JenkinsTPS/Tests/Utils/JsonUtils.h"
#include "Tests/TestUtils.h"

using namespace TPS::Test;

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();
    check(GetOwner());
    check(GetWorld());
    check(GetOwner()->InputComponent);

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Bindings.Add(MakeBindingsData());
}

void UTPSInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (!JsonUtils::WriteInputData(GenerateFileName(), InputData))
    {
        UE_LOG(LogTestUtils, Error, TEXT("Write Input Data to Json File Failed"));
    }
}

void UTPSInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Bindings.Add(MakeBindingsData());
}

FBindingsData UTPSInputRecordingComponent::MakeBindingsData() const
{
    FBindingsData BindingsData;
    BindingsData.WorldTime = GetWorld()->TimeSeconds;
    for (const auto AxisBinding : GetOwner()->InputComponent->AxisBindings)
    {
        BindingsData.AxisValues.Add(FAxisData{AxisBinding.AxisName, AxisBinding.AxisValue});
    }
    return BindingsData;
}

FString UTPSInputRecordingComponent::GenerateFileName() const
{
    FString SavedDir = FPaths::ProjectSavedDir();
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append("/Tests/").Append(FileName).Append("_").Append(Date).Append(".json");
}
