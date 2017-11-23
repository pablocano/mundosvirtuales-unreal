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

    public Plant(TargetInfo ROTargetRules)// : base(ROTargetRules)
    {
		Definitions.Add("DISABLE_LOGGER=1");
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        LoadClientLib(ROTargetRules);

        UEBuildConfiguration.bForceEnableExceptions = true;
    }

    public bool LoadClientLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(ThirdPartyPath, "Client", "lib");
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.Debug) ? "Debug" : "Release";

            Console.WriteLine(Target.Configuration);
            Console.WriteLine("... LibrariesPath -> " + Path.Combine(LibrariesPath, PlatformString, ConfigurationString));

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "clientPlant.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "utilsCore.lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Client", "src"));
        }

        Definitions.Add(string.Format("WITH_CLIENT_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
