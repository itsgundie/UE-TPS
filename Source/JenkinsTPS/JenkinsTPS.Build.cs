// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;

public class JenkinsTPS : ModuleRules
{
    public JenkinsTPS(ReadOnlyTargetRules Target) : base(Target)
    {
        Console.WriteLine("Is This Thin On?");
        Console.WriteLine(
            "Unreal version {0}.{1}.{2}", Target.Version.MajorVersion, Target.Version.MinorVersion, Target.Version.PatchVersion);
        Console.WriteLine("Target platform {0}", Target.Platform);
        Console.WriteLine("Target type {0}", Target.Type);
        Console.WriteLine("Branch name {0}", Target.Version.BranchName);
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities" });

        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.Add("HeadMountedDisplay");
        }

        PublicIncludePaths.Add("JenkinsTPS");
    }
}
