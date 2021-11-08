// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CoopGame : ModuleRules
{
	public CoopGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore", 
			"PhysicsCore",
			"UMG",
			"NavigationSystem",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"CoopGame/Public/Animations",
			"CoopGame/Public/Weapons",
			"CoopGame/Public/Components",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
