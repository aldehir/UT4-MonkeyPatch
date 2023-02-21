using System.IO;
using UnrealBuildTool;

public class MonkeyPatch : ModuleRules
{
    public MonkeyPatch(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "MonkeyPatchFuncHook",
            });
    }
}
