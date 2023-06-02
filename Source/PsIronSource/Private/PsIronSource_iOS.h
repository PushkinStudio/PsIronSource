// Copyright 2015-2023 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"
#include "PsIronSource_iOS.generated.h"

#if WITH_IRONSOURCE && PLATFORM_IOS

#import "IronSource/IronSource.h"
#import "IronSource/ISConfigurations.h"

@interface PSISDelegate : NSObject <LevelPlayRewardedVideoDelegate>

- (void)hasAvailableAdWithAdInfo:(ISAdInfo *)adInfo;
- (void)hasNoAvailableAd;
- (void)didReceiveRewardForPlacement:(ISPlacementInfo *)placementInfo withAdInfo:(ISAdInfo *)adInfo;
- (void)didFailToShowWithError:(NSError *)error andAdInfo:(ISAdInfo *)adInfo;
- (void)didOpenWithAdInfo:(ISAdInfo *)adInfo;
- (void)didCloseWithAdInfo:(ISAdInfo *)adInfo;
- (void)didClick:(ISPlacementInfo *)placementInfo withAdInfo:(ISAdInfo *)adInfo;

@property (nonatomic) UPsIronSourceProxy *Proxy;

@end

@interface PSISLogDelegate : NSObject <ISLogDelegate>

- (void)sendLog:(NSString *)log level:(ISLogLevel)level tag:(LogTag)tag;

@end

@interface PSISImpressionDataDelegate : NSObject <ISImpressionDataDelegate>

- (void)impressionDataDidSucceed:(ISImpressionData *)impressionData;

@property (nonatomic) UPsIronSourceProxy *Proxy;

@end

@interface PSISInterstitialDelegate : NSObject <LevelPlayInterstitialDelegate>

- (void)didLoadWithAdInfo:(ISAdInfo *)adInfo;
- (void)didFailToShowWithError:(NSError *)error andAdInfo:(ISAdInfo *)adInfo;
- (void)didClickWithAdInfo:(ISAdInfo *)adInfo;
- (void)didCloseWithAdInfo:(ISAdInfo *)adInfo;
- (void)didOpenWithAdInfo:(ISAdInfo *)adInfo;
- (void)didFailToLoadWithError:(NSError *)error;
- (void)didShowWithAdInfo:(ISAdInfo *)adInfo;

@property (nonatomic) FPSIronSourceVideoDelegate *PluginDelegate;

@end

@interface PSISOfferwallDelegate : NSObject <ISOfferwallDelegate>

/**
 Called after the offerwall has changed its availability.

 @param available The new offerwall availability. YES if available and ready to be shown, NO otherwise.
 */
- (void)offerwallHasChangedAvailability:(BOOL)available;

/**
 Called after the offerwall has been displayed on the screen.
 */
- (void)offerwallDidShow;

/**
 Called after the offerwall has attempted to show but failed.

 @param error The reason for the error.
 */
- (void)offerwallDidFailToShowWithError:(NSError *)error;

/**
 Called after the offerwall has been dismissed.
 */
- (void)offerwallDidClose;

/**
 @abstract Called each time the user completes an offer.
 @discussion creditInfo is a dictionary with the following key-value pairs:

 "credits" - (int) The number of credits the user has Earned since the last didReceiveOfferwallCredits event that returned YES. Note that the credits may represent multiple completions (see return parameter).

 "totalCredits" - (int) The total number of credits ever earned by the user.

 "totalCreditsFlag" - (BOOL) In some cases, we won’t be able to provide the exact amount of credits since the last event (specifically if the user clears the app’s data). In this case the ‘credits’ will be equal to the "totalCredits", and this flag will be YES.

 @param creditInfo Offerwall credit info.

 @return The publisher should return a BOOL stating if he handled this call (notified the user for example). if the return value is NO, the 'credits' value will be added to the next call.
 */
- (BOOL)didReceiveOfferwallCredits:(NSDictionary *)creditInfo;

/**
 Called after the 'offerwallCredits' method has attempted to retrieve user's credits info but failed.

 @param error The reason for the error.
 */
- (void)didFailToReceiveOfferwallCreditsWithError:(NSError *)error;

@property (nonatomic) FPSIronSourceOfferwallDelegate *PluginDelegate;

@end

#endif // WITH_IRONSOURCE && PLATFORM_IOS

UCLASS()
class UPsIronSource_iOS : public UPsIronSourceProxy
{
	GENERATED_UCLASS_BODY()

#if WITH_IRONSOURCE && PLATFORM_IOS

	// Begin UPsIronSourceProxy interface
	virtual void SetOfferwallUseClientSideCallbacks(bool bValue) override;
	virtual void InitIronSource(const FString& UserId) override;
	virtual void ForceUpdateIronSourceUser(const FString& UserId) override;
	virtual void SetSegmentInfo(const FString& SegmentName, const FString& SegmentRevenueKey, float SegmentRevenue) override;
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

	virtual bool HasOfferwall() const override;
	virtual void ShowOfferwall() const override;
	virtual void ShowOfferwallWithPlacement(const FString& PlacementName) const override;
	virtual void GetOfferwallCredits() const override;
	// End UPsIronSourceProxy interface

private:
	PSISDelegate* Delegate;
	PSISLogDelegate* LogDelegate;
	PSISImpressionDataDelegate* ImpressionDelegate;
	PSISInterstitialDelegate* InterstitialDelegate;
	PSISOfferwallDelegate* OfferwallDelegate;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
