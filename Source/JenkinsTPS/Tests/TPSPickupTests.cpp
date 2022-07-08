// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPSPickupTests.h"
#include "Misc/OutputDeviceNull.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/TPSPickupItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "JenkinsTPSCharacter.h"
#include "Components/TPSPickupComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeSpawnedInGameWorld, "TPSGame.Items.Pickup.CppActorCantBeSpawnedInGameWorld",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Pickup.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupDataShouldBeSetupCorrectly, "TPSGame.Items.Pickup.PickupDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickupCanBeTaken, "TPSGame.Items.Pickup.PickupCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryPickupHasMesh, "TPSGame.Items.Pickup.EveryPickupHasMesh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr char* PickupItemBPName = "Blueprint'/Game/Pickups/BP_TPSPickupItem.BP_TPSPickupItem'";
constexpr char* PickupItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSPickupItem.BP_Test_TPSPickupItem'";
}  // namespace

using namespace TPS::Test;

bool FCppActorCantBeSpawnedInGameWorld::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMessage =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSPickupItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarningMessage, EAutomationExpectedErrorFlags::Exact);
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{999.0f}};
    const ATPSPickupItem* PickupItem = World->SpawnActor<ATPSPickupItem>(ATPSPickupItem::StaticClass(), InitialTransform);
    if (!TestNull("Pickup item exists", PickupItem)) return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{999.0f}};
    const ATPSPickupItem* PickupItem = SpawnBlueprint<ATPSPickupItem>(World, PickupItemBPName, InitialTransform);
    if (!TestNotNull("Pickup item exists", PickupItem)) return false;

    const auto CollisionComponent = PickupItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere Collider Component Exists", CollisionComponent)) return false;

    TestTrueExpr(CollisionComponent->GetUnscaledSphereRadius() >= 20.0f);
    TestTrueExpr(CollisionComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComponent->GetGenerateOverlapEvents());
    TestTrueExpr(PickupItem->GetRootComponent() == CollisionComponent);

    ENUM_LOOP_START(ECollisionChannel, EElement)
    if (EElement != ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComponent->GetCollisionResponseToChannel(EElement) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRendererComponent = PickupItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Renderer Component Exists", TextRendererComponent)) return false;

    const auto StaticMeshComponent = PickupItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component Exists", StaticMeshComponent)) return false;
    TestTrueExpr(StaticMeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FPickupDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{999.0f}};
    ATPSPickupItem* PickupItem = SpawnBlueprint<ATPSPickupItem>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Pickup item exists", PickupItem)) return false;

    const FPickupData PickupData{EPickupItemType::SPHERE, 42};
    const FLinearColor PickupColor = FLinearColor::Yellow;
    CallFuncByNameWithParams(PickupItem, "SetPickupData", {PickupData.ToString(), PickupColor.ToString()});

    const auto TextRendererComponent = PickupItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Renderer Component Exists", TextRendererComponent)) return false;
    TestTrueExpr(TextRendererComponent->Text.ToString().Equals(FString::FromInt(PickupData.Score)));
    TestTrueExpr(TextRendererComponent->TextRenderColor == PickupColor.ToFColor(true));

    const auto StaticMeshComponent = PickupItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component Exists", StaticMeshComponent)) return false;
    const auto PickupMaterial = StaticMeshComponent->GetMaterial(0);
    if (!TestNotNull("Pickup's Material Exists", PickupMaterial)) return false;
    FLinearColor PickupMaterialColor;
    PickupMaterial->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, PickupMaterialColor);
    TestTrueExpr(PickupMaterialColor == PickupColor);

    return true;
}

bool FPickupCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    const FTransform InitialTransform{FVector{999.0f}};
    ATPSPickupItem* PickupItem = SpawnBlueprint<ATPSPickupItem>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Pickup item exists", PickupItem)) return false;

    const FPickupData PickupData{EPickupItemType::SPHERE, 42};
    const FLinearColor PickupColor = FLinearColor::Yellow;
    CallFuncByNameWithParams(PickupItem, "SetPickupData", {PickupData.ToString(), PickupColor.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, AJenkinsTPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1)) return false;
    const auto Character = Cast<AJenkinsTPSCharacter>(Pawns[0]);
    if (!TestNotNull("TPS Character Exits", Character)) return false;
    const auto PickupComponent = Character->FindComponentByClass<UTPSPickupComponent>();
    if (!TestNotNull("Pickup Component Exists", PickupComponent)) return false;
    const int PickupScoreAtStart = PickupComponent->GetItemCountByType(PickupData.Type);

    Character->SetActorLocation(InitialTransform.GetLocation());
    TestTrueExpr(PickupComponent->GetItemCountByType(PickupData.Type) == PickupScoreAtStart + PickupData.Score);
    TestTrueExpr(!IsValid(PickupItem));
    return true;
}

bool FEveryPickupHasMesh::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("Game World Exists", World)) return false;

    ENUM_LOOP_START(EPickupItemType, EElement)

    const FTransform InitialTransform{FVector{120.0f * (pickupType + 1)}};
    ATPSPickupItem* PickupItem = SpawnBlueprint<ATPSPickupItem>(World, PickupItemBPTestName, InitialTransform);
    if (!TestNotNull("Pickup item exists", PickupItem)) return false;

    const FPickupData PickupData{EElement, 42};
    const FLinearColor PickupColor = FLinearColor::Gray;
    CallFuncByNameWithParams(PickupItem, "SetPickupData", {PickupData.ToString(), PickupColor.ToString()});

    const auto StaticMeshComponent = PickupItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component Exists", StaticMeshComponent)) return false;

    const FString MeshMsg = FString::Printf(TEXT("Static Mesh for %s exists"), *UEnum::GetValueAsString(EElement));
    TestNotNull(*MeshMsg, StaticMeshComponent->GetStaticMesh());
    ENUM_LOOP_END

    return true;
}

#endif
