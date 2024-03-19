// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

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
                                "FBLPromises",
                                "../../ThirdParty/iOS/FBLPromises.embeddedframework.zip"
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
                            "VungleAdsSDK",
                            "../../ThirdParty/iOS/VungleAdsSDK.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISVungleAdapter",
                            "../../ThirdParty/iOS/ISVungleAdapter.embeddedframework.zip"
                        )
                    );

                    PublicSystemLibraries.Add("xml2");
                    PublicSystemLibraries.Add("sqlite3");
                    PublicSystemLibraries.Add("resolv");

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISMintegralAdapter",
                            "../../ThirdParty/iOS/ISMintegralAdapter.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDK",
                            "../../ThirdParty/iOS/MTGSDK.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKBanner",
                            "../../ThirdParty/iOS/MTGSDKBanner.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKBidding",
                            "../../ThirdParty/iOS/MTGSDKBidding.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKCNAddition",
                            "../../ThirdParty/iOS/MTGSDKCNAddition.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKInterstitial",
                            "../../ThirdParty/iOS/MTGSDKInterstitial.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKInterstitialVideo",
                            "../../ThirdParty/iOS/MTGSDKInterstitialVideo.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKNativeAdvanced",
                            "../../ThirdParty/iOS/MTGSDKNativeAdvanced.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKNewInterstitial",
                            "../../ThirdParty/iOS/MTGSDKNewInterstitial.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKReward",
                            "../../ThirdParty/iOS/MTGSDKReward.embeddedframework.zip"
                        )
                    );

                    // MIntegral
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "MTGSDKSplash",
                            "../../ThirdParty/iOS/MTGSDKSplash.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISFyberAdapter",
                            "../../ThirdParty/iOS/ISFyberAdapter.embeddedframework.zip"
                        )
                    );

                    // Digital Turbine (fyber)
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "IASDKCore",
                            "../../ThirdParty/iOS/IASDKCore.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISAPSAdapter",
                            "../../ThirdParty/iOS/ISAPSAdapter.embeddedframework.zip"
                        )
                    );

                    // APS
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "DTBiOSSDK",
                            "../../ThirdParty/iOS/DTBiOSSDK.embeddedframework.zip"
                        )
                    );

                    // Tapjoy
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

                    // BidMachine
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "ISBidMachineAdapter",
                            "../../ThirdParty/iOS/ISBidMachineAdapter.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BidMachine",
                            "../../ThirdParty/iOS/BidMachine.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BidMachineApiCore",
                            "../../ThirdParty/iOS/BidMachineApiCore.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BidMachineBidding",
                            "../../ThirdParty/iOS/BidMachineBidding.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "BidMachineModules",
                            "../../ThirdParty/iOS/BidMachineModules.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "OMSDK_Appodeal",
                            "../../ThirdParty/iOS/OMSDK_Appodeal.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackIABAssets",
                            "../../ThirdParty/iOS/StackIABAssets.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackMRAIDKit",
                            "../../ThirdParty/iOS/StackMRAIDKit.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackNASTKit",
                            "../../ThirdParty/iOS/StackNASTKit.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackOpenMeasure",
                            "../../ThirdParty/iOS/StackOpenMeasure.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackRichMedia",
                            "../../ThirdParty/iOS/StackRichMedia.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackVASTAssets",
                            "../../ThirdParty/iOS/StackVASTAssets.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackVASTKit",
                            "../../ThirdParty/iOS/StackVASTKit.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackVideoPlayer",
                            "../../ThirdParty/iOS/StackVideoPlayer.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackXML",
                            "../../ThirdParty/iOS/StackXML.embeddedframework.zip"
                        )
                    );
                    
                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackFoundation",
                            "../../ThirdParty/iOS/StackFoundation.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackProductPresentation",
                            "../../ThirdParty/iOS/StackProductPresentation.embeddedframework.zip"
                        )
                    );

                    PublicAdditionalFrameworks.Add(
                        new Framework(
                            "StackUIKit",
                            "../../ThirdParty/iOS/StackUIKit.embeddedframework.zip"
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
