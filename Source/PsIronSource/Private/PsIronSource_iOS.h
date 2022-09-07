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

@interface PSISInterstitialDelegate : NSObject <ISInterstitialDelegate>

- (void)interstitialDidLoad;
- (void)interstitialDidFailToShowWithError:(NSError *)error;
- (void)didClickInterstitial;
- (void)interstitialDidClose;
- (void)interstitialDidOpen;
- (void)interstitialDidFailToLoadWithError:(NSError *)error;
- (void)interstitialDidShow;

@property (nonatomic) FPSIronSourceVideoDelegate *PluginDelegate;

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

	virtual void LoadInterstitial() override;
	virtual bool IsInterstitialReady() const override;
	virtual void ShowInterstitial(const FString& PlacementName) const override;
	virtual bool IsInterstitialCappedForPlacement(const FString& PlacementName) const override;
	// End UPsIronSourceProxy interface

private:
	PSISDelegate* Delegate;
	PSISLogDelegate* LogDelegate;
	PSISImpressionDataDelegate* ImpressionDelegate;
	PSISInterstitialDelegate* InterstitialDelegate;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
