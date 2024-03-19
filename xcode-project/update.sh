#!/bin/bash
set -e

# $1 framework dir
# $2 framework name
# $3 bundle name
# $4 name to rename framework to
PackFramework()
{
	if [ ! -d "$1" ]; then
		echo "Directory $1 does not exist"
		exit 1
	fi

	EMB_FRAMEWORK=$2.embeddedframework
	FRAMEWORK=frameworks/$EMB_FRAMEWORK
	mkdir $FRAMEWORK

	if [ -d "$1/$2.xcframework" ]; then
		if [ -d "$1/$2.xcframework/ios-arm64_armv7" ]; then
	 		cp -R $1/$2.xcframework/ios-arm64_armv7/ $FRAMEWORK
	 	elif [ -d "$1/$2.xcframework/ios-arm64" ]; then
	 		cp -R $1/$2.xcframework/ios-arm64/ $FRAMEWORK
	 	fi
	else
		cp -R $1/$2.framework $FRAMEWORK
	fi

	# copy resources bundle
	if [ "$3" != "" ]
	then
		cp -R $1/$3 $FRAMEWORK
	fi

	# fix compilation (IronSource)
	if [ "$2" == "IronSource" ]
	then
		sed -i -e "s|@property (nonatomic) ISAdUnit|@property (nonatomic, retain) ISAdUnit|" $FRAMEWORK/$2.framework/Headers/ISBaseAdAdapter.h
		rm -f $FRAMEWORK/$2.framework/Headers/ISBaseAdAdapter.h-e
	fi

	# fix compilation (DTBiOSSDK)
	if [ "$2" == "DTBiOSSDK" ]
	then
		sed -i -e "s|@property NSString|@property (nonatomic, retain) NSString|g" $FRAMEWORK/$2.framework/Headers/APSEvent.h
		rm -f $FRAMEWORK/$2.framework/Headers/APSEvent.h-e
	fi

	# rename framework
	if [ "$4" != "" ]
	then
		EMB_FRAMEWORK=$4.embeddedframework
		mv $FRAMEWORK frameworks/$EMB_FRAMEWORK
		mv frameworks/$EMB_FRAMEWORK/$2.framework frameworks/$EMB_FRAMEWORK/$4.framework
		FRAMEWORK=frameworks/$EMB_FRAMEWORK
	fi

	pushd frameworks
	zip -r -y $EMB_FRAMEWORK.zip $EMB_FRAMEWORK
	rm -rf $EMB_FRAMEWORK
	popd
}

DownloadFramework()
{
	FILENAME=`basename $1`

	pushd downloads

	if [ ! -f "$FILENAME" ]; then
		echo Downloading $FILENAME...
		curl $1 --output $FILENAME
	fi

	unzip $FILENAME -d unpacked

	popd
}

rm -rf frameworks
mkdir frameworks
mkdir -p downloads
rm -rf downloads/unpacked
mkdir downloads/unpacked

DownloadFramework https://dl.google.com/googleadmobadssdk/googlemobileadssdkios-10.12.0.zip

PackFramework Pods/AppLovinSDK/applovin-ios-sdk-11.10.1 AppLovinSDK AppLovinSDKResources.bundle
PackFramework Pods/Google-Mobile-Ads-SDK/Frameworks/GoogleMobileAdsFramework GoogleMobileAds
PackFramework Pods/GoogleAppMeasurement/Frameworks GoogleAppMeasurement
# use compiled version from downloads
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-10.12.0 GoogleUtilities
# use compiled version from downloads
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-10.12.0 nanopb
# use compiled version from downloads
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-10.12.0 FBLPromises
# check google, check promises
PackFramework Pods/GoogleUserMessagingPlatform/Frameworks/Release UserMessagingPlatform
PackFramework Pods/IronSourceAdMobAdapter/ISAdMobAdapter ISAdMobAdapter ISAdMobResources.bundle
PackFramework Pods/IronSourceAppLovinAdapter/ISAppLovinAdapter ISAppLovinAdapter
PackFramework Pods/IronSourceFacebookAdapter/ISFacebookAdapter ISFacebookAdapter
# mintegral begin
PackFramework Pods/IronSourceMintegralAdapter/ISMintegralAdapter ISMintegralAdapter
PackFramework Pods/MintegralAdSDK/Fmk MTGSDK
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKBanner
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKBidding
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKCNAddition
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKInterstitial
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKInterstitialVideo
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKNativeAdvanced
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKNewInterstitial
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKReward
PackFramework Pods/MintegralAdSDK/Fmk MTGSDKSplash
# mintegral end
PackFramework Pods/IronSourceSDK/IronSource IronSource
PackFramework Pods/IronSourceUnityAdsAdapter/ISUnityAdsAdapter ISUnityAdsAdapter
PackFramework Pods/IronSourceVungleAdapter/ISVungleAdapter ISVungleAdapter
PackFramework Pods/UnityAds UnityAds
PackFramework Pods/VungleAds/static VungleAdsSDK
# Digital Turbine (fyber)
PackFramework Pods/IronSourceFyberAdapter/ISFyberAdapter ISFyberAdapter
PackFramework Pods/Fyber_Marketplace_SDK/IASDKCore IASDKCore
# APS
PackFramework Pods/IronSourceAPSAdapter/ISAPSAdapter ISAPSAdapter
PackFramework Pods/AmazonPublisherServicesSDK/APS_iOS_SDK-4.7.2 DTBiOSSDK
# Tapjoy
PackFramework Pods/IronSourceTapjoyAdapter/ISTapjoyAdapter ISTapjoyAdapter
PackFramework Pods/TapjoySDK/TapjoySDK_iOS_v13.0.1/Libraries Tapjoy
# BidMachine
PackFramework Pods/IronSourceBidMachineAdapter/ISBidMachineAdapter ISBidMachineAdapter
PackFramework Pods/BidMachine BidMachine
PackFramework Pods/BidMachine BidMachineApiCore
PackFramework Pods/BidMachine BidMachineBidding
PackFramework Pods/BidMachine BidMachineModules
PackFramework Pods/OMSDK_Appodeal OMSDK_Appodeal
PackFramework Pods/StackIAB StackIABAssets
PackFramework Pods/StackIAB StackMRAIDKit
PackFramework Pods/StackIAB StackNASTKit
PackFramework Pods/StackIAB StackOpenMeasure
PackFramework Pods/StackIAB StackRichMedia
PackFramework Pods/StackIAB StackVASTAssets
PackFramework Pods/StackIAB StackVASTKit
PackFramework Pods/StackIAB StackVideoPlayer
PackFramework Pods/StackIAB StackXML
PackFramework Pods/StackModules StackFoundation
PackFramework Pods/StackModules StackProductPresentation
PackFramework Pods/StackModules StackUIKit
