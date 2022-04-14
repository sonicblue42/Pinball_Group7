// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PinballGroup7 : ModuleRules
{
	public PinballGroup7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
