// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class JenkinsTPSTarget : TargetRules
{
    public JenkinsTPSTarget(TargetInfo Target) : base(Target)
    {
        Console.WriteLine("Game Target CS output");
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("JenkinsTPS");
    }
}
