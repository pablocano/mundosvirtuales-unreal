// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class Plant : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public Plant(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        LoadCoreLib(Target);
    }

    public bool LoadCoreLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(ThirdPartyPath, "PlantCore", "Libraries");
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.Debug) ? "Debug" : "Release";
  
            Console.WriteLine("... LibrariesPath -> " + Path.Combine(LibrariesPath, PlatformString, ConfigurationString));
            
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "plantCorelib.lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "PlantCore", "Includes"));
        }

        Definitions.Add(string.Format("WITH_PLANT_CORE_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    public bool LoadThirdPartyLibrary(TargetInfo Target, string LibName)
    {
        bool isLibrarySupported = false;

        string LibrariesPath = Path.Combine(ThirdPartyPath, LibName, "Libraries");
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
        string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.Debug) ? "Debug" : "Release";


        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, LibName + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, LibName, "Includes"));
        }

        Definitions.Add(string.Format("WITH_" + LibName.ToUpper() + "_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
