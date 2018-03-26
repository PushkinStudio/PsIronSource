// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"

#if WITH_IRONSOURCE && PLATFORM_IOS
#import "IronSource/IronSource.h"
#endif // WITH_IRONSOURCE && PLATFORM_IOS

UIronSource_iOS::UIronSource_iOS(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_IRONSOURCE && PLATFORM_IOS

@implementation ISDelegate
// This method lets you know whether or not there is a video
// ready to be presented. It is only after this method is invoked
// with 'hasAvailableAds' set to 'YES' that you can should 'showRV'.
- (void)rewardedVideoHasChangedAvailability:(BOOL)available
{
    NSLog(@"%s", __PRETTY_FUNCTION__);
}

// This method gets invoked after the user has been rewarded.
- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::ReceivedReward);
    }
}

// This method gets invoked when there is a problem playing the video.
// If it does happen, check out 'error' for more information and consult
// our knowledge center for help.
- (void)rewardedVideoDidFailToShowWithError:(NSError *)error
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoShowFailed);
    }
}

// This method gets invoked when we take control, but before
// the video has started playing.
- (void)rewardedVideoDidOpen
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoOpened);
    }
}

// This method gets invoked when we return controlback to your hands.
// We chose to notify you about rewards here and not in 'didReceiveRewardForPlacement'.
// This is because reward can occur in the middle of the video.
- (void)rewardedVideoDidClose
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoClosed);
    }
}

// This method gets invoked when the video has started playing.
- (void)rewardedVideoDidStart
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoStarted);
    }
}

// This method gets invoked when the video has stopped playing.
- (void)rewardedVideoDidEnd
{
    NSLog(@"%s", __PRETTY_FUNCTION__);

    if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoEnded);
    }
}

// Called after a video has been clicked.
- (void)didClickRewardedVideo:(ISPlacementInfo *)placementInfo
{
	NSLog(@"%s", __PRETTY_FUNCTION__);

	if (self.PluginDelegate != nullptr)
    {
    	self.PluginDelegate->Broadcast(EIronSourceEventType::VideoTapped);
    }
}
@end

void UIronSource_iOS::InitIronSource(const FString& UserId)
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Initialize IronSource with iOS SDK"), *VA_FUNC_LINE);

	if (bIronSourceInitialized)
	{
		UE_LOG(LogIronSource, Error, TEXT("%s: Trying to initialize IronSource when it's already been initialized!"), *VA_FUNC_LINE);
		return;
	}

	NSString* UserIdNativeString = UserId.GetNSString();
	NSString* AppKeyNativeString = GetDefault<UIronSourceSettings>()->IronSourceIOSAppKey.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
		Delegate = [[ISDelegate alloc] init];
		Delegate.PluginDelegate = &VideoStateDelegate;

		[IronSource setRewardedVideoDelegate:Delegate];
		[IronSource setUserId:UserIdNativeString];
		[IronSource initWithAppKey:AppKeyNativeString];
		[ISIntegrationHelper validateIntegration];
	});

	bIronSourceInitialized = true;
}

bool UIronSource_iOS::HasRewardedVideo() const
{
	return [IronSource hasRewardedVideo];
}

FString UIronSource_iOS::GetPlacementRewardName(const FString& PlacementName) const
{
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	ISPlacementInfo* Info = [IronSource rewardedVideoPlacementInfo:PlacementNameNativeString];
	if (Info != nil)
	{
		return FString(Info.rewardName);
	}

	return FString();
}

FString UIronSource_iOS::GetPlacementRewardAmount(const FString& PlacementName) const
{
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	ISPlacementInfo* Info = [IronSource rewardedVideoPlacementInfo:PlacementNameNativeString];
	if (Info != nil)
	{
		return FString(Info.rewardAmount.stringValue);
	}

	return FString();
}

bool UIronSource_iOS::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	NSString* PlacementNameNativeString = PlacementName.GetNSString();
	return [IronSource isRewardedVideoCappedForPlacement:PlacementNameNativeString];
}

void UIronSource_iOS::ShowRewardedVideo(const FString& PlacementName) const
{
	NSString* PlacementNameNativeString = PlacementName.GetNSString();

	dispatch_async(dispatch_get_main_queue(), ^{
		[IronSource showRewardedVideoWithViewController:[UIApplication sharedApplication].keyWindow.rootViewController placement:PlacementNameNativeString];
	});
}

#endif // WITH_IRONSOURCE && PLATFORM_IOS
