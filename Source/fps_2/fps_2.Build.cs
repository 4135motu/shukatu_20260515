// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class fps_2 : ModuleRules
{
	public fps_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" ,"AIModule",
    "GameplayTasks",
    "NavigationSystem","LevelSequence",
"MovieScene"});
	}
}
