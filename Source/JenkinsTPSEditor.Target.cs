// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class JenkinsTPSEditorTarget : TargetRules
{
    public JenkinsTPSEditorTarget(TargetInfo Target) : base(Target)
    {
        Console.WriteLine("Editor Target CS output");
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("JenkinsTPS");
    }
}
