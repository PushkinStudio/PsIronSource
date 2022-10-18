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
	 	cp -R $1/$2.xcframework/ios-arm64_armv7/ $FRAMEWORK
	else
		cp -R $1/$2.framework $FRAMEWORK
	fi

	# copy resources bundle
	if [ "$3" != "" ]
	then
		cp -R $1/$3 $FRAMEWORK
	fi

	# fix compilation
	if [ "$2" == "IronSource" ]
	then
		sed -i -e "s|@property (nonatomic) ISAdUnit|@property (nonatomic, retain) ISAdUnit|" $FRAMEWORK/$2.framework/Headers/ISBaseAdAdapter.h
		rm -f $FRAMEWORK/$2.framework/Headers/ISBaseAdAdapter.h-e
	fi

	if [ "$2" == "YandexMobileMetrica" ]
	then
		rm -f $FRAMEWORK/$2.framework/PrivateHeaders
		rm -rf $FRAMEWORK/$2.framework/Versions/A/PrivateHeaders
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

DownloadFramework https://dl.google.com/googleadmobadssdk/googlemobileadssdkios-9.9.0.zip
DownloadFramework https://s3.amazonaws.com/ssa.public/ironsource-mobile/ios-adapters/pangle/Pangle_iOS_SDK_4.5.2.5.zip

pod install

PackFramework Pods/AdColony AdColony
PackFramework Pods/Ads-Global/SDK BUAdSDK
PackFramework Pods/Ads-Global/SDK BUCNAuxiliary
PackFramework Pods/Ads-Global/SDK BUFoundation
PackFramework Pods/Ads-Global/SDK BURelyAdSDK
PackFramework Pods/Ads-Global/SDK BUVAAuxiliary
PackFramework Pods/Ads-Global/SDK PAGAdSDK PAGAdSDK.bundle
PackFramework downloads/unpacked/SDK BURelyFoundation_Global
PackFramework Pods/AppLovinSDK/applovin-ios-sdk-11.4.3 AppLovinSDK AppLovinSDKResources.bundle
PackFramework Pods/ChartboostSDK ChartboostSDK
PackFramework Pods/Google-Mobile-Ads-SDK/Frameworks/GoogleMobileAdsFramework-Current GoogleMobileAds
PackFramework Pods/GoogleAppMeasurement/Frameworks GoogleAppMeasurement "" GoogleAppMeasurement_is
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-9.9.0 GoogleUtilities "" GoogleUtilities_is
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-9.9.0 nanopb "" nanopb_is
PackFramework downloads/unpacked/GoogleMobileAdsSdkiOS-9.9.0 PromisesObjC "" PromisesObjC_is
PackFramework Pods/GoogleUserMessagingPlatform/Frameworks/Release UserMessagingPlatform
PackFramework Pods/IronSourceAdColonyAdapter/ISAdColonyAdapter ISAdColonyAdapter
PackFramework Pods/IronSourceAdMobAdapter/ISAdMobAdapter ISAdMobAdapter
PackFramework Pods/IronSourceAppLovinAdapter/ISAppLovinAdapter ISAppLovinAdapter
PackFramework Pods/IronSourceChartboostAdapter/ISChartboostAdapter ISChartboostAdapter
PackFramework Pods/IronSourceFacebookAdapter/ISFacebookAdapter ISFacebookAdapter
PackFramework Pods/IronSourceMyTargetAdapter/ISMyTargetAdapter ISMyTargetAdapter
PackFramework Pods/IronSourcePangleAdapter/ISPangleAdapter ISPangleAdapter
PackFramework Pods/IronSourceSDK/IronSource IronSource
PackFramework Pods/IronSourceTapjoyAdapter/ISTapjoyAdapter ISTapjoyAdapter
PackFramework Pods/IronSourceUnityAdsAdapter/ISUnityAdsAdapter ISUnityAdsAdapter
PackFramework Pods/IronSourceVungleAdapter/ISVungleAdapter ISVungleAdapter
PackFramework Pods/myTargetSDK MyTargetSDK
PackFramework Pods/TapjoySDK/TapjoySDK_iOS_v12.10.0/Libraries Tapjoy
PackFramework Pods/UnityAds UnityAds
PackFramework Pods/VungleSDK-iOS VungleSDK
PackFramework Pods/YandexMobileAds/static YandexMobileAds YandexMobileAds.xcframework/YandexMobileAdsBundle.bundle
PackFramework Pods/YandexMobileMetrica/static YandexMobileMetrica
PackFramework Pods/IronSourceYandexMobileAdsAdapters IronSourceYandexMobileAdsAdapters
