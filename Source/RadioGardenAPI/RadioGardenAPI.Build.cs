using UnrealBuildTool;

public class RadioGardenAPI : ModuleRules
{
    public RadioGardenAPI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "Json", "HTTP"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "JsonUtilities"
        });
    }
}
