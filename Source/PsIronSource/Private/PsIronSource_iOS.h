// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"
#include "PsIronSource_iOS.generated.h"

#if WITH_IRONSOURCE && PLATFORM_IOS

#import "IronSource/IronSource.h"

@interface PSISDelegate : NSObject <ISRewardedVideoDelegate>

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

@interface PSISLogDelegate : NSObject <ISLogDelegate>

- (void)sendLog:(NSString *)log level:(ISLogLevel)level tag:(LogTag)tag;

@end

@interface PSISImpressionDataDelegate : NSObject <ISImpressionDataDelegate>

- (void)impressionDataDidSucceed:(ISImpressionData *)impressionData;

@property (nonatomic) UPsIronSourceProxy *Proxy;

@end

#endif // WITH_IRONSOURCE && PLATFORM_IOS

UCLASS()
class UPsIronSource_iOS : public UPsIronSourceProxy
{
	GENERATED_UCLASS_BODY()

#if WITH_IRONSOURCE && PLATFORM_IOS

	// Begin UPsIronSourceProxy interface
	virtual void InitIronSource(const FString& UserId) override;
	virtual void ForceUpdateIronSourceUser(const FString& UserId) override;
	virtual bool HasRewardedVideo() const override;
	virtual FString GetPlacementRewardName(const FString& PlacementName) const override;
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const override;
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const override;
	virtual void ShowRewardedVideo(const FString& PlacementName) const override;
	virtual void SetGDPRConsent(bool bConsent) const override;
	// End UPsIronSourceProxy interface

private:
	PSISDelegate* Delegate;
	PSISLogDelegate* LogDelegate;
	PSISImpressionDataDelegate* ImpressionDelegate;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
