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

    private void CopyToBinaries(string Filepath, TargetInfo Target)
    {
        string binariesDir = Path.Combine(GetUProjectPath(), "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
    }

    public bool LoadCoreLib(TargetInfo Target)
    {
        bool isLibrarySupported = false;
		string LibrariesPathPQXX = Path.Combine(ThirdPartyPath, "PlantCore", "Postgres", "Windows");
		string LibrariesPathLibPQXX = Path.Combine(LibrariesPathPQXX, "lib");

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string LibrariesPath = Path.Combine(ThirdPartyPath, "PlantCore", "Libraries");
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.Debug) ? "Debug" : "Release";

            Console.WriteLine("... LibrariesPath -> " + Path.Combine(LibrariesPath, PlatformString, ConfigurationString));

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, PlatformString, ConfigurationString, "plantCorelib.lib"));
			
			CopyToBinaries(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxx.dll"), Target);
            CopyToBinaries(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxxD.dll"), Target);
			
            if (Target.Configuration == UnrealTargetConfiguration.Debug)
            {
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxxD.dll")));
                //PublicAdditionalLibraries.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxxD.dll"));
                //PublicDelayLoadDLLs.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxxD.dll"));
                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxxD.lib"));
            }
            else
            {
                RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxx.dll")));
                //PublicAdditionalLibraries.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxx.dll"));
                //PublicDelayLoadDLLs.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxx.dll"));
                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPathLibPQXX, PlatformString, "libpqxx.lib"));
            }
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "PlantCore", "Includes"));
            PublicIncludePaths.Add(Path.Combine(LibrariesPathPQXX, "include"));
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
