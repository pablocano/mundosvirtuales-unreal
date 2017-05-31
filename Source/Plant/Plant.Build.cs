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

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        UEBuildConfiguration.bForceEnableExceptions = true;

        LoadCoreLib(Target);
    }

    public string GetUProjectPath()
    {
        return Directory.GetParent(Directory.GetParent(ModuleDirectory).ToString()).ToString();
    }

    private void addedDLL(string Filepath, TargetInfo Target)
    {
        string binariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);

        RuntimeDependencies.Add(new RuntimeDependency(Filepath));
    }

    public bool LoadCoreLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;
		string LibrariesPathSOCI = Path.Combine(ThirdPartyPath, "PlantCore", "Utils", "SOCI", "Windows");
		string LibrariesPathLibSOCI = Path.Combine(LibrariesPathSOCI, "lib");

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(ThirdPartyPath, "PlantCore", "Libraries");
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.DebugGame) ? "Debug" : "Release";

            Console.WriteLine("... LibrariesPath -> " + Path.Combine(LibrariesPath, PlatformString, ConfigurationString));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "plantCorelib.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "utilsCore.lib"));

			// SOCI Library
			
			// Added DLLs
			Console.WriteLine("... Copy SOCI Libraries");
            addedDLL(Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString, "soci_core_4_0.dll"), Target);
            addedDLL(Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString, "soci_empty_4_0.dll"), Target);
            addedDLL(Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString, "soci_odbc_4_0.dll"), Target);
            addedDLL(Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString, "soci_postgresql_4_0.dll"), Target);
			addedDLL(Path.Combine(ThirdPartyPath, "PlantCore", "Utils", "PostgreSQL", "lib",  "libpq.dll"), Target);
			

            // Added libray SOCI
            Console.WriteLine("... LibrariesPath -> " + Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPathLibSOCI, PlatformString, ConfigurationString, "libsoci_core_4_0.lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "PlantCore", "Includes"));
            
			// Include SOCI library
			PublicIncludePaths.Add(Path.Combine(LibrariesPathSOCI, "include"));
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
