// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#include "PsIronSource_iOS.h"

UPsIronSource_iOS::UPsIronSource_iOS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_IRONSOURCE && PLATFORM_IOS
	bTapjoyInitialized = false;
#endif // WITH_IRONSOURCE && PLATFORM_IOS
}

#if WITH_IRONSOURCE && PLATFORM_IOS

#include "PsIronSourceDefines.h"
#include "PsIronSourceSettings.h"
#import <DTBiOSSDK/DTBiOSSDK.h>

@implementation PSISDelegate

// Called after a rewarded video has changed its availability to true.
// @param adInfo The info of the ad.
// Replaces the delegate rewardedVideoHasChangedAvailability:(true)available
- (void)hasAvailableAdWithAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	FPsIronSourceAdInfo AdInfo;
	if (adInfo != nil)
	{
		AdInfo.AuctionId = FString(adInfo.auction_id);
		AdInfo.AdUnit = FString(adInfo.ad_unit);
		AdInfo.AdNetwork = FString(adInfo.ad_network);
		AdInfo.InstanceName = FString(adInfo.instance_name);
		AdInfo.InstanceId = FString(adInfo.instance_id);
		AdInfo.Country = FString(adInfo.country);
		AdInfo.Revenue = [adInfo.revenue floatValue];
		AdInfo.Precision = FString(adInfo.precision);
		AdInfo.Ab = FString(adInfo.ab);
		AdInfo.SegmentName = FString(adInfo.segment_name);
		AdInfo.LifetimeRevenue = [adInfo.lifetime_revenue floatValue];
		AdInfo.EncryptedCpm = FString(adInfo.encrypted_cpm);
		AdInfo.bInitialized = true;
	}

	AsyncTask(ENamedThreads::GameThread, [self, AdInfo]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->SetAdInfo(AdInfo);
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoAvailable);
		}
	});
}

// Called after a rewarded video has changed its availability to false.
// Replaces the delegate rewardedVideoHasChangedAvailability:(false)available
- (void)hasNoAvailableAd
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoNotAvailable);
		}
	});
}

// Called after a rewarded video has been viewed completely and the user is eligible for a reward.
// @param placementInfo An object that contains the placement's reward name and amount.
// @param adInfo The info of the ad.
- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo withAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::ReceivedReward);
		}
	});
}

// Called after a rewarded video has attempted to show but failed.
// @param error The reason for the error
// @param adInfo The info of the ad.
- (void)didFailToShowWithError:(NSError*)error andAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoShowFailed);
		}
	});
}

// Called after a rewarded video has been opened.
// @param adInfo The info of the ad.
- (void)didOpenWithAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoOpened);
		}
	});
}

// Called after a rewarded video has been dismissed.
// @param adInfo The info of the ad.
- (void)didCloseWithAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoClosed);
		}
	});
}

// Called after a rewarded video has been clicked.
// This callback is not supported by all networks, and we recommend using it
// only if it's supported by all networks you included in your build
// @param adInfo The info of the ad.
- (void)didClick:(ISPlacementInfo*)placementInfo withAdInfo:(ISAdInfo*)adInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoTapped);
		}
	});
}

@end

@implementation PSISLogDelegate

- (void)sendLog:(NSString*)log level:(ISLogLevel)level tag:(LogTag)tag
{
	NSLog(@"%@", log);
}

@end

@implementation PSISImpressionDataDelegate

// Called after an impression
- (void)impressionDataDidSucceed:(ISImpressionData*)impressionData
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	FPsIronSourceImpressionData Data;
	if (impressionData != nil)
	{
		Data.AuctionId = FString(impressionData.auction_id);
		Data.AdUnit = FString(impressionData.ad_unit);
		Data.AdNetwork = FString(impressionData.ad_network);
		Data.InstanceName = FString(impressionData.instance_name);
		Data.InstanceId = FString(impressionData.instance_id);
		Data.Country = FString(impressionData.country);
		Data.Placement = FString(impressionData.placement);
		Data.Revenue = [impressionData.revenue floatValue];
		Data.Precision = FString(impressionData.precision);
		Data.Ab = FString(impressionData.ab);
		Data.SegmentName = FString(impressionData.segment_name);
		Data.LifetimeRevenue = [impressionData.lifetime_revenue floatValue];
		Data.EncryptedCpm = FString(impressionData.encrypted_cpm);
		Data.ConversionValue = [impressionData.conversion_value floatValue];
	}

	AsyncTask(ENamedThreads::GameThread, [self, Data]() {
		if (self.Proxy != nullptr)
		{
			self.Proxy->SetImpressionData(Data);
			self.Proxy->VideoStateDelegate.Broadcast(EIronSourceEventType::Impression);
		}
	});
}

@end

@implementation PSISInterstitialDelegate

/**
 Called after an interstitial has been loaded
 @param adInfo The info of the ad.
 */
- (void)didLoadWithAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialReady);
		}
	});
}

/**
	Called after an interstitial has attempted to show but failed.
	@param error The reason for the error
	@param adInfo The info of the ad.
 */
- (void)didFailToShowWithError:(NSError*)error andAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialShowFailed);
		}
	});
}

/**
	Called after an interstitial has been clicked.
	@param adInfo The info of the ad.
 */
- (void)didClickWithAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialClicked);
		}
	});
}

/**
	Called after an interstitial has been dismissed.
	@param adInfo The info of the ad.
 */
- (void)didCloseWithAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialClosed);
		}
	});
}

/**
	Called after an interstitial has been opened.
	This is the indication for impression.
	@param adInfo The info of the ad.
 */
- (void)didOpenWithAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialOpened);
		}
	});
}

/**
	Called after an interstitial has attempted to load but failed.
	@param error The reason for the error
 */
- (void)didFailToLoadWithError:(NSError*)error;
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialLoadFailed);
		}
	});
}

/**
	Called after an interstitial has been displayed on the screen.
	This callback is not supported by all networks, and we recommend using it
	only if it's supported by all networks you included in your build.
	@param adInfo The info of the ad.
 */
- (void)didShowWithAdInfo:(ISAdInfo*)adInfo;
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialShowSucceeded);
		}
	});
}

@end

@implementation PSTapjoyConnectionDelegate

- (void)tjcConnectSuccess:(NSNotification *)notifyObj
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	self.connectionEstablished = true;
	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.ISObj != nullptr)
		{
			self.ISObj->OnTapjoyConnected(true);
		}
	});
}

- (void)tjcConnectFail:(NSNotification *)notifyObj
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	self.connectionEstablished = false;
	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.ISObj != nullptr)
		{
			self.ISObj->OnTapjoyConnected(false);
		}
	});
}

@end

@implementation PSTapjoyPlacementDelegate

- (void)requestDidSucceed:(TJPlacement *)placement
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)requestDidFail:(TJPlacement *)placement error:(nullable NSError *)error
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)contentIsReady:(TJPlacement *)placement
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)contentDidAppear:(TJPlacement *)placement
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Opened, TEXT(""), 0);
		}
	});
}

- (void)contentDidDisappear:(TJPlacement *)placement
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Closed, TEXT(""), 0);
		}
	});
	[placement requestContent];
}

- (void)didClick:(TJPlacement *)placement
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)placement:(TJPlacement *)placement
didRequestPurchase:(nullable TJActionRequest *)request
		productId:(nullable NSString *)productId
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

- (void)placement:(TJPlacement *)placement
 didRequestReward:(nullable TJActionRequest *)request
		   itemId:(nullable NSString *)itemId
		 quantity:(int)quantity
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	// ZEN-23134 - check and improve task reward handling
	AsyncTask(ENamedThreads::GameThread, [self, itemId, quantity]() {
		if (self.PluginDelegate != nullptr)
		{
			const FString ItemId = FString(itemId);
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Rewarded, ItemId, quantity);
		}
	});
}

@end

void UPsIronSource_iOS::SetOfferwallUseClientSideCallbacks(bool bValue)
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s: SetOfferwallUseClientSideCallbacks IronSource with iOS SDK"), *PS_FUNC_LINE);

	if (bIronSourceInitialized)
	{
		UE_LOG(LogPsIronSource, Warning, TEXT("%s: Trying SetOfferwallUseClientSideCallbacks when IronSource already initialized!"), *PS_FUNC_LINE);
		return;
	}

	[ISSupersonicAdsConfiguration configurations].useClientSideCallbacks = [NSNumber numberWithBool:bValue];
}

void UPsIronSource_iOS::InitIronSource(const FString& UserId, bool bOfferwallEnable)
{
	if (bOfferwallEnable)
	{
		InitOfferwall();
	}

	UE_LOG(LogPsIronSource, Log, TEXT("%s: Initialize IronSource with iOS SDK"), *PS_FUNC_LINE);
	if (bIronSourceInitialized)
	{
		UE_LOG(LogPsIronSource, Warning, TEXT("%s: Trying to initialize IronSource when it's already been initialized!"), *PS_FUNC_LINE);
		return;
	}

	NSString* UserIdNativeString = UserId.GetNSString();
	NSString* AppKeyNativeString = GetDefault<UPsIronSourceSettings>()->IronSourceIOSAppKey.GetNSString();

	// Init APS SDK before IS (see https://developers.is.com/ironsource-mobile/ios/aps-integration-guide/#step-6)
	NSString* APSIOSAppIdNativeString = nil;
	if (GetDefault<UPsIronSourceSettings>()->APSIOSAppId.IsEmpty() == false)
	{
		APSIOSAppIdNativeString = GetDefault<UPsIronSourceSettings>()->APSIOSAppId.GetNSString();
	}

	dispatch_async(dispatch_get_main_queue(), ^{
	  if (APSIOSAppIdNativeString != nil)
	  {
		  [[DTBAds sharedInstance] setAppKey:APSIOSAppIdNativeString];
		  UE_LOG(LogPsIronSource, Log, TEXT("%s: APS is initalized"), *PS_FUNC_LINE);
	  }
	  else
	  {
		  UE_LOG(LogPsIronSource, Warning, TEXT("%s: APS is NOT initalized"), *PS_FUNC_LINE);
	  }

	  Delegate = [[PSISDelegate alloc] init];
	  Delegate.Proxy = this;

	  LogDelegate = [[PSISLogDelegate alloc] init];

	  ImpressionDelegate = [[PSISImpressionDataDelegate alloc] init];
	  ImpressionDelegate.Proxy = this;

	  InterstitialDelegate = [[PSISInterstitialDelegate alloc] init];
	  InterstitialDelegate.PluginDelegate = &VideoStateDelegate;

	  [IronSource setLogDelegate:LogDelegate];
	  [IronSource setLevelPlayRewardedVideoDelegate:Delegate];
	  [IronSource addImpressionDataDelegate:ImpressionDelegate];
	  [IronSource setLevelPlayInterstitialDelegate:InterstitialDelegate];
	  [IronSource setUserId:UserIdNativeString];
	  [IronSource initWithAppKey:AppKeyNativeString];
	  [ISIntegrationHelper validateIntegration];
	});

	bIronSourceInitialized = true;
}

void UPsIronSource_iOS::ForceUpdateIronSourceUser(const FString& UserId)
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s: set new userid for Ironscouce"), *PS_FUNC_LINE);

	if (!bIronSourceInitialized)
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource userid when it's not yet initialized!"), *PS_FUNC_LINE);
		return;
	}

	NSString* UserIdNativeString = UserId.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource setDynamicUserId:UserIdNativeString];
	});
}

void UPsIronSource_iOS::SetSegmentInfo(const FString& SegmentName, const FString& SegmentRevenueKey, float SegmentRevenue)
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s: set new segment name for Ironscouce"), *PS_FUNC_LINE);

	if (!bIronSourceInitialized)
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment name when it's not yet initialized!"), *PS_FUNC_LINE);
		return;
	}

	if (SegmentName.IsEmpty())
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment name but it's empty!"), *PS_FUNC_LINE);
		return;
	}

	if (SegmentRevenueKey.IsEmpty())
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment revenue but field key is empty!"), *PS_FUNC_LINE);
		return;
	}

	if (FMath::IsNegativeFloat(SegmentRevenue))
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment revenue but it's negative '%f'!"), *PS_FUNC_LINE, SegmentRevenue);
		return;
	}

	const FString SegmentRevenueString = FString::SanitizeFloat(SegmentRevenue);
	NSString* SegmentNameNativeString = SegmentName.GetNSString();
	NSString* SegmentRevenueKeyNativeString = SegmentRevenueKey.GetNSString();
	NSString* SegmentRevenueNativeString = SegmentRevenueString.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
	  ISSegment* Segment = [[ISSegment alloc] init];
	  [Segment setSegmentName:SegmentNameNativeString];
	  [Segment setCustomValue:SegmentRevenueNativeString forKey:SegmentRevenueKeyNativeString];

	  [IronSource setSegment:Segment];
	});
}

bool UPsIronSource_iOS::HasRewardedVideo() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	return [IronSource hasRewardedVideo];
}

FString UPsIronSource_iOS::GetPlacementRewardName(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	ISPlacementInfo* Info = [IronSource rewardedVideoPlacementInfo:PlacementNameNativeString];
	if (Info != nil)
	{
		return FString(Info.rewardName);
	}

	return FString();
}

FString UPsIronSource_iOS::GetPlacementRewardAmount(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	ISPlacementInfo* Info = [IronSource rewardedVideoPlacementInfo:PlacementNameNativeString];
	if (Info != nil)
	{
		return FString(Info.rewardAmount.stringValue);
	}

	return FString();
}

bool UPsIronSource_iOS::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	return [IronSource isRewardedVideoCappedForPlacement:PlacementNameNativeString];
}

void UPsIronSource_iOS::ShowRewardedVideo(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource showRewardedVideoWithViewController:UIApplication.sharedApplication.delegate.window.rootViewController placement:PlacementNameNativeString];
	});
}

void UPsIronSource_iOS::SetConsent(bool bConsent) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource setConsent:bConsent];
	});
}

void UPsIronSource_iOS::SetOfferwallConsent(const FPsOfferwallPrivacyPolicy& OfferwallPP) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);

	bool bSubjectToGDPR = OfferwallPP.bSubjectToGDPR;
	NSString* CCPASettingString = OfferwallPP.CCPASetting.GetNSString();
	NSString* UserConsentString = OfferwallPP.bUserConsent ? @"1" : @"0";

	dispatch_async(dispatch_get_main_queue(), ^{
	  TJPrivacyPolicy *privacyPolicy = [Tapjoy getPrivacyPolicy];
	  [privacyPolicy setSubjectToGDPR:bSubjectToGDPR];
	  [privacyPolicy setUserConsent:UserConsentString];
	  [privacyPolicy setUSPrivacy:CCPASettingString];
	});
}

void UPsIronSource_iOS::LoadInterstitial()
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource loadInterstitial];
	});
}

bool UPsIronSource_iOS::IsInterstitialReady() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	return [IronSource hasInterstitial];
}

void UPsIronSource_iOS::ShowInterstitial(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource showInterstitialWithViewController:UIApplication.sharedApplication.delegate.window.rootViewController placement:PlacementNameNativeString];
	});
}

bool UPsIronSource_iOS::IsInterstitialCappedForPlacement(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	return [IronSource isInterstitialCappedForPlacement:PlacementNameNativeString];
}

bool UPsIronSource_iOS::HasOfferwall() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	return TapjoyConnectionDelegate != nil && TapjoyConnectionDelegate.connectionEstablished &&
		TapjoyPlacement != nil && TapjoyPlacement.isContentReady;
}

void UPsIronSource_iOS::ShowOfferwall() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	if (HasOfferwall())
	{
		dispatch_async(dispatch_get_main_queue(), ^{
		  [TapjoyPlacement showContentWithViewController:nil];
		});
	}
}

void UPsIronSource_iOS::InitOfferwall()
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s: Initialize Tapjoy"), *PS_FUNC_LINE);
	if (bTapjoyInitialized)
	{
		UE_LOG(LogPsIronSource, Warning, TEXT("%s: Trying to initialize Tapjoy when it's already been initialized!"), *PS_FUNC_LINE);
		return;
	}

	if (GetDefault<UPsIronSourceSettings>()->TapjoyAPIKeyIOS.IsEmpty() == false)
	{
		bTapjoyInitialized = true;

		NSString* TapjoyAPIKeyIOSString = GetDefault<UPsIronSourceSettings>()->TapjoyAPIKeyIOS.GetNSString();
		dispatch_async(dispatch_get_main_queue(), ^{
		  TapjoyConnectionDelegate = [[PSTapjoyConnectionDelegate alloc] init];
		  TapjoyConnectionDelegate.ISObj = this;

		  [[NSNotificationCenter defaultCenter] addObserver:TapjoyConnectionDelegate selector:@selector(tjcConnectSuccess:) name:TJC_CONNECT_SUCCESS object:nil];
		  [[NSNotificationCenter defaultCenter] addObserver:TapjoyConnectionDelegate selector:@selector(tjcConnectFail:) name:TJC_CONNECT_FAILED object:nil];

		  [Tapjoy connect:TapjoyAPIKeyIOSString];
		});
	}
	else
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: no TapjoyAPIKeyIOS provided"), *PS_FUNC_LINE);
	}
}

void UPsIronSource_iOS::OnTapjoyConnected(bool bSuccess)
{
	if (bSuccess)
	{
		OnOfferwallConnected.Broadcast();
		if (GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement.IsEmpty() == false)
		{
			NSString* TapjoyOfferwallPlacementString = GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement.GetNSString();
			dispatch_async(dispatch_get_main_queue(), ^{
			  TapjoyPlacementDelegate = [[PSTapjoyPlacementDelegate alloc] init];
			  TapjoyPlacementDelegate.PluginDelegate = &OfferwallStateDelegate;

			  TapjoyPlacement = [TJPlacement placementWithName:TapjoyOfferwallPlacementString delegate:TapjoyPlacementDelegate];
			  [TapjoyPlacement requestContent];
			});
		}
		else
		{
			UE_LOG(LogPsIronSource, Error, TEXT("%s: no TapjoyOfferwallPlacement provided"), *PS_FUNC_LINE);
			bTapjoyInitialized = false;
		}
	}
	else
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Error establishing connection with Tapjoy!"), *PS_FUNC_LINE);
		bTapjoyInitialized = false;
	}
}

#endif // WITH_IRONSOURCE && PLATFORM_IOS
