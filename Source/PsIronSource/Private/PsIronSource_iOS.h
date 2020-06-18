// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"
#include "PsIronSource_iOS.generated.h"

#if WITH_IRONSOURCE && PLATFORM_IOS

#import "IronSource/IronSource.h"

@interface PSISVideoDelegate : NSObject <ISRewardedVideoDelegate>

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

@interface PSISInterstitialDelegate : NSObject <ISInterstitialDelegate>

- (void)interstitialDidFailToLoadWithError:(NSError *)error;
- (void)interstitialDidFailToShowWithError:(NSError *)error;
- (void)interstitialDidLoad;
- (void)interstitialDidShow;
- (void)didClickInterstitial;
- (void)interstitialDidClose;
- (void)interstitialDidOpen;

@property (nonatomic) FPSIronSourceInterstitialDelegate *PluginDelegate;

@end

@interface PSISLogDelegate : NSObject <ISLogDelegate>

- (void)sendLog:(NSString *)log level:(ISLogLevel)level tag:(LogTag)tag;

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
	virtual void SetGDPRConsent(bool bConsent) const override;
	
	virtual bool HasRewardedVideo() const override;
	virtual FString GetPlacementRewardName(const FString& PlacementName) const override;
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const override;
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const override;
	virtual void ShowRewardedVideo(const FString& PlacementName) const override;

	virtual void LoadInterstitial() const override;
	virtual bool IsInterstitialReady() const override;
	virtual bool IsInterstitialCappedForPlacement(const FString& PlacementName) const override;
	virtual void ShowInterstitial(const FString& PlacementName) const override;

	// End UPsIronSourceProxy interface

private:
	PSISVideoDelegate* VideoDelegate;
	PSISInterstitialDelegate* InterstitialDelegate;
	PSISLogDelegate* LogDelegate;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
