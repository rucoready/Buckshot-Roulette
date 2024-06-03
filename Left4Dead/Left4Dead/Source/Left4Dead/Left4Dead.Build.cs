// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Left4Dead : ModuleRules
{
	public Left4Dead(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","Niagara" });
	}
}
