// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "IronSourceProxy.h"
#include "IronSource_iOS.generated.h"

#if WITH_IRONSOURCE && PLATFORM_IOS

#import "IronSource/IronSource.h"

@interface ISDelegate : NSObject <ISRewardedVideoDelegate>

- (void)rewardedVideoHasChangedAvailability:(BOOL)available;
- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo;
- (void)rewardedVideoDidFailToShowWithError:(NSError *)error;
- (void)rewardedVideoDidOpen;
- (void)rewardedVideoDidClose;
- (void)rewardedVideoDidStart;
- (void)rewardedVideoDidEnd;
- (void)didClickRewardedVideo:(ISPlacementInfo *)placementInfo;

@property (nonatomic) FPSIronSourceVideoDelegate *PluginDelegate;

@end

#endif // WITH_IRONSOURCE && PLATFORM_IOS

UCLASS()
class UIronSource_iOS : public UIronSourceProxy
{
	GENERATED_UCLASS_BODY()

#if WITH_IRONSOURCE && PLATFORM_IOS

	// Begin UIronSourceProxy interface
	virtual void InitIronSource(const FString& UserId) override;
	virtual bool HasRewardedVideo() const override;
	virtual FString GetPlacementRewardName(const FString& PlacementName) const override;
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const override;
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const override;
	virtual void ShowRewardedVideo(const FString& PlacementName) const override;
	// End UIronSourceProxy interface

private:
	ISDelegate* Delegate;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
