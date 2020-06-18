// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#include "PsIronSource_iOS.h"

#include "PsIronSourceDefines.h"
#include "PsIronSourceSettings.h"

#include "Async/Async.h"

UPsIronSource_iOS::UPsIronSource_iOS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_IRONSOURCE && PLATFORM_IOS

@implementation PSISVideoDelegate
// This method lets you know whether or not there is a video
// ready to be presented. It is only after this method is invoked
// with 'hasAvailableAds' set to 'YES' that you can should 'showRV'.
- (void)rewardedVideoHasChangedAvailability:(BOOL)available
{
	NSLog(@"%s", __PRETTY_FUNCTION__);
}

// This method gets invoked after the user has been rewarded.
- (void)didReceiveRewardForPlacement:(ISPlacementInfo*)placementInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::ReceivedReward);
		}
	});
}

// This method gets invoked when there is a problem playing the video.
// If it does happen, check out 'error' for more information and consult
// our knowledge center for help.
- (void)rewardedVideoDidFailToShowWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.localizedDescription);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoShowFailed);
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
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoOpened);
		}
	});
}

// This method gets invoked when we return controlback to your hands.
// We chose to notify you about rewards here and not in 'didReceiveRewardForPlacement'.
// This is because reward can occur in the middle of the video.
- (void)rewardedVideoDidClose
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoClosed);
		}
	});
}

// This method gets invoked when the video has started playing.
- (void)rewardedVideoDidStart
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoStarted);
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
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoEnded);
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
			self.PluginDelegate->Broadcast(EIronSourceVideoEventType::VideoTapped);
		}
	});
}
@end

@implementation PSISInterstitialDelegate

// Invoked when there is no Interstitial Ad available after calling load function.
// If it does happen, check out 'error' for more information and consult
// our knowledge center for help.
- (void)interstitialDidFailToLoadWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.localizedDescription);

	AsyncTask(ENamedThreads::GameThread, [self]() {
        if (self.PluginDelegate != nullptr)
        {
            self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialLoadFailed);
        }
    });
}

// Called if showing the Interstitial for the user has failed.
// If it does happen, check out 'error' for more information and consult
// our knowledge center for help.
- (void)interstitialDidFailToShowWithError:(NSError*)error
{
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.localizedDescription);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialShowFailed);
		}
	});
}

// Invoked when Interstitial Ad is ready to be shown after load function was called.
- (void)interstitialDidLoad
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialReady);
		}
	});
}

// Called each time the Interstitial window has opened successfully.
- (void)interstitialDidShow
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialSucceeded);
		}
	});
}

// Called each time the end user has clicked on the Interstitial ad.
- (void)didClickInterstitial
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialTapped);
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
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialClosed);
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
			self.PluginDelegate->Broadcast(EIronSourceInterstitialEventType::InterstitialOpened);
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
      VideoDelegate = [[PSISVideoDelegate alloc] init];
      VideoDelegate.PluginDelegate = &VideoStateDelegate;

      InterstitialDelegate = [[PSISInterstitialDelegate alloc] init];
      InterstitialDelegate.PluginDelegate = &InterstitialStateDelegate;
        
	  LogDelegate = [[PSISLogDelegate alloc] init];

	  [IronSource setLogDelegate:LogDelegate];
	  [IronSource setRewardedVideoDelegate:VideoDelegate];
      [IronSource setInterstitialDelegate:InterstitialDelegate];
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

void UPsIronSource_iOS::SetGDPRConsent(bool bConsent) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
      [IronSource setConsent:bConsent];
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
	  [IronSource showRewardedVideoWithViewController:[UIApplication sharedApplication].keyWindow.rootViewController placement:PlacementNameNativeString];
	});
}

void UPsIronSource_iOS::LoadInterstitial() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	return [IronSource loadInterstitial];
}

bool UPsIronSource_iOS::IsInterstitialReady() const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	return [IronSource hasInterstitial];
}

bool UPsIronSource_iOS::IsInterstitialCappedForPlacement(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	return [IronSource isInterstitialCappedForPlacement:PlacementNameNativeString];
}

void UPsIronSource_iOS::ShowInterstitial(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	NSString* PlacementNameNativeString = PlacementName.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
      [IronSource showInterstitialWithViewController:[UIApplication sharedApplication].keyWindow.rootViewController placement:PlacementNameNativeString];
    });
}

#endif // WITH_IRONSOURCE && PLATFORM_IOS
