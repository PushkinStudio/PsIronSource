// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

using System.IO;
using Tools.DotNETCommon;

namespace UnrealBuildTool.Rules
{
    public class PsIronSource : ModuleRules
    {
        public PsIronSource(ReadOnlyTargetRules Target) : base(Target)
        {
            bool bIronSourceEnable = false;
            ConfigHierarchy EngineConfig = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, DirectoryReference.FromFile(Target.ProjectFile), Target.Platform);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceEnable", out bIronSourceEnable);
            PublicDefinitions.Add("WITH_IRONSOURCE=" + (bIronSourceEnable ? "1" : "0"));

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

            if (bIronSourceEnable)
            {
                if (Target.Platform == UnrealTargetPlatform.IOS)
                {
                    string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                    AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "PsIronSource_UPL_IOS.xml"));

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "IronSource",
                            "../../ThirdParty/iOS/IronSource.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISFacebookAdapter",
                            "../../ThirdParty/iOS/ISFacebookAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "UnityAds",
                            "../../ThirdParty/iOS/UnityAds.embeddedframework.zip"
                        )
                    );
                    
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISUnityAdsAdapter",
                            "../../ThirdParty/iOS/ISUnityAdsAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISAdMobAdapter",
                            "../../ThirdParty/iOS/ISAdMobAdapter.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "GoogleAppMeasurement",
                            "../../ThirdParty/iOS/GoogleAppMeasurement_is.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "GoogleMobileAds",
                            "../../ThirdParty/iOS/GoogleMobileAds.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "GoogleUtilities",
                            "../../ThirdParty/iOS/GoogleUtilities_is.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "nanopb",
                            "../../ThirdParty/iOS/nanopb_is.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "PromisesObjC",
                            "../../ThirdParty/iOS/PromisesObjC_is.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "UserMessagingPlatform",
                            "../../ThirdParty/iOS/UserMessagingPlatform.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISAppLovinAdapter",
                            "../../ThirdParty/iOS/ISAppLovinAdapter.embeddedframework.zip"
                        )
                    );

                    // Dependence for ISAppLovinAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "AppLovinSDK",
                            "../../ThirdParty/iOS/AppLovinSDK.embeddedframework.zip",
                            "AppLovinSDKResources.bundle"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "VungleSDK",
                            "../../ThirdParty/iOS/VungleSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISVungleAdapter",
                            "../../ThirdParty/iOS/ISVungleAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "AdColony",
                            "../../ThirdParty/iOS/AdColony.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISAdColonyAdapter",
                            "../../ThirdParty/iOS/ISAdColonyAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "Tapjoy",
                            "../../ThirdParty/iOS/Tapjoy.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISTapjoyAdapter",
                            "../../ThirdParty/iOS/ISTapjoyAdapter.embeddedframework.zip"
                        )
                    );

                    PublicSystemLibraries.Add("xml2");
                    PublicSystemLibraries.Add("sqlite3");
                    PublicSystemLibraries.Add("resolv");

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "Chartboost",
                            "../../ThirdParty/iOS/Chartboost.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISChartboostAdapter",
                            "../../ThirdParty/iOS/ISChartboostAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISMyTargetAdapter",
                            "../../ThirdParty/iOS/ISMyTargetAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MyTargetSDK",
                            "../../ThirdParty/iOS/MyTargetSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISPangleAdapter",
                            "../../ThirdParty/iOS/ISPangleAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BUAdSDK",
                            "../../ThirdParty/iOS/BUAdSDK.embeddedframework.zip",
                            "BUAdSDK.bundle"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BUFoundation",
                            "../../ThirdParty/iOS/BUFoundation.embeddedframework.zip"
                        )
                    );

                    PublicSystemLibraries.Add("z");
                    PublicSystemLibraries.Add("bz2");
                }
                else if (Target.Platform == UnrealTargetPlatform.Android)
                {
                    string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                    AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "PsIronSource_UPL_Android.xml"));
                }
            }
        }
    }
}
