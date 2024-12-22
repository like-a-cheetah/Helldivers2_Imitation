// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HellDivers2 : ModuleRules
{
	public HellDivers2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "HellDivers2"
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",
            "AIModule",
            "GameplayTasks",
            "UMG",
            "Niagara",
            "Cascade",
            "NavigationSystem",
            "EnhancedInput",
            "Slate",
            "SlateCore",
            "LevelSequence",
            "MovieScene"

        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
        PrivateIncludePaths.Add(ModuleDirectory);
        PublicIncludePaths.Add(ModuleDirectory);
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
