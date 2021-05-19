// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

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

	AsyncTask(ENamedThreads::GameThread, [self]() {
		if (self.PluginDelegate != nullptr)
		{
			self.PluginDelegate->Broadcast(EIronSourceEventType::VideoAvailable);
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
	NSLog(@"%s %@", __PRETTY_FUNCTION__, error.localizedDescription);

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
//to include in your build.
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

	  [IronSource setLogDelegate:LogDelegate];
	  [IronSource setRewardedVideoDelegate:Delegate];
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
	  [IronSource showRewardedVideoWithViewController:[UIApplication sharedApplication].keyWindow.rootViewController placement:PlacementNameNativeString];
	});
}

void UPsIronSource_iOS::SetGDPRConsent(bool bConsent) const
{
	UE_LOG(LogPsIronSource, Log, TEXT("%s"), *PS_FUNC_LINE);
	dispatch_async(dispatch_get_main_queue(), ^{
	  [IronSource setConsent:bConsent];
	});
}

#endif // WITH_IRONSOURCE && PLATFORM_IOS
