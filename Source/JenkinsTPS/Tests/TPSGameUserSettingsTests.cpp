// My game copyright

#include "Settings/TPSGameUserSettings.h"
#if WITH_AUTOMATION_TESTS

#include "JenkinsTPS/Tests/TPSGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "JenkinsTPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExists, "TPSGame.GameUserSettings.SettingsShouldExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FSettingsShouldExists::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/MainMap.MainMap");
    TestTrueExpr(UTPSGameUserSettings::Get() != nullptr);
    TestTrueExpr(UTPSGameUserSettings::Get()->GetVideoSettings().Num() == 5);
    return true;
}

#endif