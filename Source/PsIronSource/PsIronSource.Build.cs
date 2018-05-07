// Copyright 2017 Pushkin Studio. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
    public class PsIronSource : ModuleRules
    {
        public PsIronSource(ReadOnlyTargetRules Target) : base(Target)
        {
            const bool bEnableIronSource = true;
            PublicDefinitions.Add("WITH_IRONSOURCE=" + (bEnableIronSource ? "1" : "0"));

            PrivateIncludePaths.AddRange(
                new string[] {
                    "PsIronSource/Private",
                });

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                });

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings",
                }
            );

            if (bEnableIronSource)
            {
                if (Target.Platform == UnrealTargetPlatform.IOS)
                {
                    PublicAdditionalFrameworks.Add(
                        new UEBuildFramework(
                            "IronSource",
                            "../../ThirdParty/iOS/IronSource.embeddedframework.zip"
                        )
                        
                    );

                    PublicAdditionalFrameworks.Add(
                        new UEBuildFramework(
                            "ISFacebookAdapter",
                            "../../ThirdParty/iOS/ISFacebookAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new UEBuildFramework(
                            "ISTapjoyAdapter",
                            "../../ThirdParty/iOS/ISTapjoyAdapter.embeddedframework.zip"
                        )
                    );
                    
                    PublicAdditionalFrameworks.Add(
                        new UEBuildFramework(
                            "ISUnityAdsAdapter",
                            "../../ThirdParty/iOS/ISUnityAdsAdapter.embeddedframework.zip"
                        )
                    );
                }
                else if (Target.Platform == UnrealTargetPlatform.Android)
                {
                    string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                    AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "IronSource_APL.xml"));
                }
            }
        }
    }
}