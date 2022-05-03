// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSPickupTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/TPSPickupItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeSpawnedInGameWorld, "TPSGame.Items.Pickup.CppActorCantBeSpawnedInGameWorld",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Pickup.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace 
{
constexpr  char* PickupItemBPName = "Blueprint'/Game/Pickups/BP_TPSPickupItem.BP_TPSPickupItem'";
}

using namespace TPS::Test;

bool FCppActorCantBeSpawnedInGameWorld::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMessage = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSPickupItem::StaticClass()->GetName());
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
    if (!TestNotNull("Text Renderer Component Exists",TextRendererComponent)) return false;

    const auto StaticMeshComponent = PickupItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component Exists",StaticMeshComponent)) return false;
    TestTrueExpr(StaticMeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

#endif