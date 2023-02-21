using System.IO;
using UnrealBuildTool;

public class MonkeyPatchFuncHook : ModuleRules
{
    public MonkeyPatchFuncHook(TargetInfo Target)
    {
        Type = ModuleType.External;

        string IncludeDir = ModuleDirectory + "/funchook-1.1.1/include";
        string LibDir = ModuleDirectory + "/Lib/";
        string Platform;

        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
        {
            LibDir += "VS" + WindowsPlatform.GetVisualStudioCompilerVersionName() + "/";
        }

        switch (Target.Platform)
        {
            case UnrealTargetPlatform.Win64:
                Platform = "Win64";
                break;
            default:
                return;
        }

        LibDir += Platform;

        PublicIncludePaths.Add(IncludeDir);
        PublicLibraryPaths.Add(LibDir);

        PublicAdditionalLibraries.Add("psapi.lib");
        PublicAdditionalLibraries.Add("distorm.lib");
        PublicAdditionalLibraries.Add("funchook.lib");
    }
}
