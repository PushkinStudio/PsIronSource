// Copyright 2015-2023 MY.GAMES. All Rights Reserved.

using System.IO;
using Tools.DotNETCommon;

namespace UnrealBuildTool.Rules
{
    public class PsIronSource : ModuleRules
    {
        public PsIronSource(ReadOnlyTargetRules Target) : base(Target)
        {                    
            bool bIronSourceEnable = false;
            bool bIronSourceDontLinkPromisesObjC = false;
            bool bIronSourceDontLinkGoogleAppMeasurement = false;
            bool bIronSourceDontLinkGoogleUtilities = false;
            bool bIronSourceDontLinkNanopb = false;

            ConfigHierarchy EngineConfig = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, DirectoryReference.FromFile(Target.ProjectFile), Target.Platform);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceEnable", out bIronSourceEnable);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceDontLinkPromisesObjC", out bIronSourceDontLinkPromisesObjC);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceDontLinkGoogleAppMeasurement", out bIronSourceDontLinkGoogleAppMeasurement);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceDontLinkGoogleUtilities", out bIronSourceDontLinkGoogleUtilities);
            EngineConfig.TryGetValue("/Script/PsIronSource.PsIronSourceSettings", "bIronSourceDontLinkNanopb", out bIronSourceDontLinkNanopb);

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

                    if (bIronSourceDontLinkGoogleAppMeasurement == false)
                    {
                        // Dependence for ISAdMobAdapter
                        PublicAdditionalFrameworks.Add(
                            new Framework(
                                "GoogleAppMeasurement",
                                "../../ThirdParty/iOS/GoogleAppMeasurement.embeddedframework.zip"
                            )
                        );
                    }

                    // Dependence for ISAdMobAdapter
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "GoogleMobileAds",
                            "../../ThirdParty/iOS/GoogleMobileAds.embeddedframework.zip"
                        )
                    );

                    if (bIronSourceDontLinkGoogleUtilities == false)
                    {
                        // Dependence for ISAdMobAdapter
                        PublicAdditionalFrameworks.Add(
                            new Framework(
                                "GoogleUtilities",
                                "../../ThirdParty/iOS/GoogleUtilities.embeddedframework.zip"
                            )
                        );
                    }

                    if (bIronSourceDontLinkNanopb == false)
                    {
                        // Dependence for ISAdMobAdapter
                        PublicAdditionalFrameworks.Add(
                            new Framework(
                                "nanopb",
                                "../../ThirdParty/iOS/nanopb.embeddedframework.zip"
                            )
                        );
                    }

                    if (bIronSourceDontLinkPromisesObjC == false)
                    {
                        // Dependence for ISAdMobAdapter
                        PublicAdditionalFrameworks.Add(
                            new Framework(
                                "PromisesObjC",
                                "../../ThirdParty/iOS/PromisesObjC.embeddedframework.zip"
                            )
                        );
                    }

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
                            "ChartboostSDK",
                            "../../ThirdParty/iOS/ChartboostSDK.embeddedframework.zip"
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
                            "ISPangleAdapter",
                            "../../ThirdParty/iOS/ISPangleAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BUAdSDK",
                            "../../ThirdParty/iOS/BUAdSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BURelyAdSDK",
                            "../../ThirdParty/iOS/BURelyAdSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "PAGAdSDK",
                            "../../ThirdParty/iOS/PAGAdSDK.embeddedframework.zip",
                            "PAGAdSDK.bundle"
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
                            "MTGSDK",
                            "../../ThirdParty/iOS/MTGSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKBanner",
                            "../../ThirdParty/iOS/MTGSDKBanner.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKReward",
                            "../../ThirdParty/iOS/MTGSDKReward.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKBidding",
                            "../../ThirdParty/iOS/MTGSDKBidding.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKNewInterstitial",
                            "../../ThirdParty/iOS/MTGSDKNewInterstitial.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKInterstitialVideo",
                            "../../ThirdParty/iOS/MTGSDKInterstitialVideo.embeddedframework.zip"
                        )
                    );


                    // Static libs linking
                    string StaticLibsPath = Path.Combine(ModuleDirectory, "../../", "ThirdParty/iOS/StaticLibs");
                    string StaticLibHeaders = "Headers";

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalAFN", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalAFN",
                            "libBURelyFoundationGlobalAFN.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalFoundation", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalFoundation", 
                            "libBURelyFoundationGlobalFoundation.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalGecko", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalGecko", 
                            "libBURelyFoundationGlobalGecko.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalHeader", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalHeader", 
                            "libBURelyFoundationGlobalHeader.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalNETWork", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalNETWork", 
                            "libBURelyFoundationGlobalNETWork.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalSDWebImage", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalSDWebImage", 
                            "libBURelyFoundationGlobalSDWebImage.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalYYModel", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalYYModel", 
                            "libBURelyFoundationGlobalYYModel.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalZFPlayer", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalZFPlayer", 
                            "libBURelyFoundationGlobalZFPlayer.a"
                        )
                    );

                    PublicIncludePaths.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalZip", StaticLibHeaders));
                    PublicAdditionalLibraries.Add(Path.Combine(StaticLibsPath, "BURelyFoundationGlobalZip", 
                            "libBURelyFoundationGlobalZip.a"
                        )
                    );

                    string SDKROOT = Utils.RunLocalProcessAndReturnStdOut("/usr/bin/xcrun", "--sdk iphoneos --show-sdk-path");
                    PublicSystemLibraryPaths.Add(SDKROOT + "/usr/lib/swift");
                    PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphoneos");
                    PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-5.0/iphoneos");
                    PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-5.5/iphoneos");

                    PublicSystemLibraries.Add("z");
                    PublicSystemLibraries.Add("bz2");
                    PublicSystemLibraries.Add("c++abi");
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
