// My game copyright

#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "JenkinsTPS/Tests/TestUtils.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/TPSGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Settings/TPSGameSetting.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseShouldBeVisibleOnGamePaused, "TPSGame.UI.PauseShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseShouldBeCollapsedOnGameUnPaused, "TPSGame.UI.PauseShouldBeCollapsedOnGameUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVideoSettingsShouldExist, "TPSGame.UI.AllVideoSettingsShouldExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsCanBeApplied, "TPSGame.UI.SettingsCanBeApplied",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
void PausePressed(UInputComponent* InputComponent)
{
    if (!InputComponent) return;

    const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, "ToogleGamePause", EInputEvent::IE_Pressed);
    if (ActionIndex != INDEX_NONE)
    {
        const auto JumpActionBind = InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::P);
    }
}

template <class T>
T* FindWidgetByClass()
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetTestGameWorld(), Widgets, T::StaticClass(), false);
    return Widgets.Num() != 0 ? Cast<T>(Widgets[0]) : nullptr;
}

UWidget* FindWidgetByName(const UUserWidget* ParentWidget, const FName& SearchingWidgetName)
{
    if (!ParentWidget || !ParentWidget->WidgetTree) return nullptr;

    UWidget* FoundWidget = nullptr;
    UWidgetTree::ForWidgetAndChildren(ParentWidget->WidgetTree->RootWidget, [&](UWidget* Child)
    {
        if (Child && Child->GetFName().IsEqual(SearchingWidgetName))
        {
            FoundWidget = Child;
        }
    });
    return FoundWidget;
}

void NextSettingClick(int32 SettingIndex)
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const auto* SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
    const auto* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
    NextSettingButton->OnClicked.Broadcast();
}
}

bool FPauseShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    const APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PlayerController->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);

    return true;
}

bool FPauseShouldBeCollapsedOnGameUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    const APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PlayerController->InputComponent);
    PausePressed(PlayerController->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    return true;
}

bool FAllVideoSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BenchmarkButton != nullptr);

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VerticalBox != nullptr);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSettings.Num());
    TestTrueExpr(VerticalBox->GetChildrenCount() == 5);

    return true;
}

bool FSettingsCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    APlayerController* PlayerController = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PlayerController != nullptr);
    PausePressed(PlayerController->InputComponent);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    const auto TexQualityBefore =  UTPSGameUserSettings::Get()->GetTextureQuality();
    TestTrueExpr(TexQualityBefore == VideoSettings[1]->GetCurrentOption().Value);

    NextSettingClick(1);
    const auto TexQualityAfter =  UTPSGameUserSettings::Get()->GetTextureQuality();
    TestTrueExpr(TexQualityBefore != TexQualityAfter);
    TestTrueExpr(TexQualityAfter == VideoSettings[1]->GetCurrentOption().Value);

    UTPSGameUserSettings::Get()->SetTextureQuality(TexQualityBefore);
    
    return true;
}

#endif
