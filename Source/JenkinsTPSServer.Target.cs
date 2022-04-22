// My game copyright
using UnrealBuildTool;
using System.Collections.Generic;
public class JenkinsTPSServerTarget : TargetRules
{
    public JenkinsTPSServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        ExtraModuleNames.AddRange(new string[] { "JenkinsTPS" });
    }
}
