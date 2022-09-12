// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#include "PsIronSource_iOS.h"

UPsIronSource_iOS::UPsIronSource_iOS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_IRONSOURCE && PLATFORM_IOS

#include "PsIronSourceDefines.h"
#include "PsIronSourceSettings.h"

@implementation PSISDelegate
// This method lets you know whether or not there is a video
// ready to be presented. It is only after this method is invoked
// with 'hasAvailableAds' set to 'YES' that you can should 'showRV'.
- (void)rewardedVideoHasChangedAvailability:(BOOL)available
{
	NSLog(@"%s %d", __PRETTY_FUNCTION__, available);

	AsyncTask(ENamedThreads::GameThread, [self, available]() {
		if (self.PluginDelegate != nullptr)
		{
			if (available)
			{
				self.PluginDelegate->Broadcast(EIronSourceEventType::VideoAvailable);
			}
			else
			{
				self.PluginDelegate->Broadcast(EIronSourceEventType::VideoNotAvailable);
			}
		}
	});
}

// This method gets invoked after the user has been rewarded.
- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::ReceivedReward);
		}
	});
}

// This method gets invoked when there is a problem playing the video.
// If it does happen, check out 'error' for more information and consult
// our knowledge center for help.
- (void)rewardedVideoDidFailToShowWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoShowFailed);
		}
	});
}

// This method gets invoked when we take control, but before
// the video has started playing.
- (void)rewardedVideoDidOpen
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoOpened);
		}
	});
}

// This method gets invoked when we return control back to your hands.
- (void)rewardedVideoDidClose
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoClosed);
		}
	});
}

// This method gets invoked when the video has started playing.
// Note: the events DidStart & DidEnd below are not available for all supported rewarded video ad networks. Check which events are available per ad network you choose
// to include in your build.
// We recommend only using events which register to ALL ad networks you //include in your build.
- (void)rewardedVideoDidStart
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoStarted);
		}
	});
}

// This method gets invoked when the video has stopped playing.
- (void)rewardedVideoDidEnd
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoEnded);
		}
	});
}

// Called after a video has been clicked.
- (void)didClickRewardedVideo:(ISPlacementInfo*)placementInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoTapped);
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

// Invoked when Interstitial Ad is ready to be shown after load function was //called.
- (void)interstitialDidLoad
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialReady);
		}
	});
}
// Called if showing the Interstitial for the user has failed.
// You can learn about the reason by examining the ‘error’ value
- (void)interstitialDidFailToShowWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialShowFailed);
		}
	});
}
// Called each time the end user has clicked on the Interstitial ad, for supported networks only
- (void)didClickInterstitial
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialClicked);
		}
	});
}
// Called each time the Interstitial window is about to close
- (void)interstitialDidClose
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialClosed);
		}
	});
}
// Called each time the Interstitial window is about to open
- (void)interstitialDidOpen
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialOpened);
		}
	});
}
// Invoked when there is no Interstitial Ad available after calling load function.
// @param error - will contain the failure code and description.
- (void)interstitialDidFailToLoadWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::InterstitialLoadFailed);
		}
	});
}

// Invoked right before the Interstitial screen is about to open.
// NOTE - This event is available only for some of the networks.
// You should NOT treat this event as an interstitial impression, but rather use InterstitialAdOpenedEvent
- (void)interstitialDidShow
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

@implementation PSISOfferwallDelegate

- (void)offerwallHasChangedAvailability:(BOOL)available
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self, available]() {
		if (self.PluginDelegate != nullptr)
		{
			if (available)
			{
				self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Available, 0, 0, false);
			}
			else
			{
				self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::NotAvailable, 0, 0, false);
			}
		}
	});
}

- (void)offerwallDidShow
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Opened, 0, 0, false);
		}
	});
}

- (void)offerwallDidFailToShowWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::ShowFailed, 0, 0, false);
		}
	});
}

- (void)offerwallDidClose
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Closed, 0, 0, false);
		}
	});
}

- (BOOL)didReceiveOfferwallCredits:(NSDictionary*)creditInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	const int32 Credits = [[creditInfo objectForKey:@"credits"] intValue];
	const int32 TotalCredits = [[creditInfo objectForKey:@"totalCredits"] intValue];
	const bool TotalCreditsFlag = [[creditInfo objectForKey:@"totalCreditsFlag"] boolValue];

	AsyncTask(ENamedThreads::GameThread, [self, Credits, TotalCredits, TotalCreditsFlag]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::Credited, Credits, TotalCredits, TotalCreditsFlag);
		}
	});

	return YES;
}

- (void)didFailToReceiveOfferwallCreditsWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.description);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceOfferwallEventType::GetCreditsFailed, 0, 0, false);
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

void UPsIronSource_iOS::InitIronSource(const FString& UserId)
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s: Initialize IronSource with iOS SDK"), *PS_FUNC_LINE);

	if (bIronSourceInitialized)
	{
		UE_LOG(LogPsIronSource, Warning, TEXT("%s: Trying to initialize IronSource when it's already been initialized!"), *PS_FUNC_LINE);
		return;
	}

	NSString* UserIdNativeString = UserId.GetNSString();
	NSString* AppKeyNativeString = GetDefault<UPsIronSourceSettings>()->IronSourceIOSAppKey.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
	  Delegate = [[PSISDelegate alloc] init];
	  Delegate.PluginDelegate = &VideoStateDelegate;

	  LogDelegate = [[PSISLogDelegate alloc] init];

	  ImpressionDelegate = [[PSISImpressionDataDelegate alloc] init];
	  ImpressionDelegate.Proxy = this;

	  InterstitialDelegate = [[PSISInterstitialDelegate alloc] init];
	  InterstitialDelegate.PluginDelegate = &VideoStateDelegate;

	  OfferwallDelegate = [[PSISOfferwallDelegate alloc] init];
	  OfferwallDelegate.PluginDelegate = &OfferwallStateDelegate;

	  [IronSource setLogDelegate:LogDelegate];
	  [IronSource setRewardedVideoDelegate:Delegate];
	  [IronSource addImpressionDataDelegate:ImpressionDelegate];
	  [IronSource setInterstitialDelegate:InterstitialDelegate];
	  [IronSource setOfferwallDelegate:OfferwallDelegate];
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

void UPsIronSource_iOS::SetGDPRConsent(bool bConsent) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource setConsent:bConsent];
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
	return [IronSource hasOfferwall];
}

void UPsIronSource_iOS::ShowOfferwall() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource showOfferwallWithViewController:UIApplication.sharedApplication.delegate.window.rootViewController];
	});
}

void UPsIronSource_iOS::ShowOfferwallWithPlacement(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource showOfferwallWithViewController:UIApplication.sharedApplication.delegate.window.rootViewController placement:PlacementNameNativeString];
	});
}

void UPsIronSource_iOS::GetOfferwallCredits() const
{
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource offerwallCredits];
	});
}

#endif // WITH_IRONSOURCE && PLATFORM_IOS
