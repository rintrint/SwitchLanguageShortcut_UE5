using UnrealBuildTool;

public class SwitchLanguageShortcut : ModuleRules
{
    public SwitchLanguageShortcut(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "Projects",
                "ApplicationCore",
            }
        );

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd",
                    "LevelEditor",
                    "EditorStyle",
                    "ToolMenus",
                }
            );
        }
    }
}
