// My game copyright
using UnrealBuildTool;
using System.Collections.Generic;
public class JenkinsTPSClientTarget : TargetRules
{
    public JenkinsTPSClientTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Client;
        ExtraModuleNames.AddRange(new string[] { "JenkinsTPS" });
    }
}
