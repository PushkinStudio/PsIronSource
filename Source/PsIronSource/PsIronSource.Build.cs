// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

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
                            "../../ThirdParty/iOS/Tapjoy.embeddedframework.zip",
                            "TapjoyResources.bundle"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISTapjoyAdapter",
                            "../../ThirdParty/iOS/ISTapjoyAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "InMobiSDK",
                            "../../ThirdParty/iOS/InMobiSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "INMMoatMobileAppKit",
                            "../../ThirdParty/iOS/INMMoatMobileAppKit.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISInMobiAdapter",
                            "../../ThirdParty/iOS/ISInMobiAdapter.embeddedframework.zip"
                        )
                    );

                    PublicSystemLibraries.Add("xml2");
                    PublicSystemLibraries.Add("sqlite3");

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISTikTokAdapter",
                            "../../ThirdParty/iOS/ISTikTokAdapter.embeddedframework.zip",
                            "BUAdSDK.bundle"
                        )
                    );

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
                            "ISMintegralAdapter",
                            "../../ThirdParty/iOS/ISMintegralAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISMyTargetAdapter",
                            "../../ThirdParty/iOS/ISMyTargetAdapter.embeddedframework.zip"
                        )
                    );
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
