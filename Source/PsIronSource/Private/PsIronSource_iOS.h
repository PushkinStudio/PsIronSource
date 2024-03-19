// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"
#include "PsIronSource_iOS.generated.h"

#if WITH_IRONSOURCE && PLATFORM_IOS

#import "IronSource/IronSource.h"
#import "IronSource/ISConfigurations.h"
#import "Tapjoy/Tapjoy.h"

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

@interface PSTapjoyConnectionDelegate: NSObject

- (void)tjcConnectSuccess:(NSNotification *)notifyObj;
- (void)tjcConnectFail:(NSNotification *)notifyObj;

@property (nonatomic) bool connectionEstablished;

@property (nonatomic) UPsIronSource_iOS *ISObj;

@end

@interface PSTapjoyPlacementDelegate : NSObject <TJPlacementDelegate>
/**
 * Callback issued by TJ to publisher to state that placement request is successful
 * @param placement The TJPlacement that was sent
 */
- (void)requestDidSucceed:(TJPlacement *)placement;

/**
 * Called when an error occurs while sending the placement
 * @param placement The TJPlacement that was sent
 * @error error code
 */
- (void)requestDidFail:(TJPlacement *)placement error:(nullable NSError *)error;

/**
 * Called when content for an placement is successfully cached
 * @param placement The TJPlacement that was sent
 */
- (void)contentIsReady:(TJPlacement *)placement;

/**
 * Called when placement content did appear
 * @param placement The TJPlacement that was sent
 */
- (void)contentDidAppear:(TJPlacement *)placement;

/**
 * Called when placement content did disappear
 * @param placement The TJPlacement that was sent
 */
- (void)contentDidDisappear:(TJPlacement *)placement;

/**
 * Called when a click event has occurred
 * @param placement The TJPlacement that was sent
 */
- (void)didClick:(TJPlacement *)placement;

/**
 * Callback issued by TJ to publisher when the user has successfully completed a purchase request
 * @param request - The TJActionRequest object
 * @param productId - the id of the offer that sent the request
 */
- (void)placement:(TJPlacement *)placement
didRequestPurchase:(nullable TJActionRequest *)request
		productId:(nullable NSString *)productId;

/**
 * Callback issued by TJ to publisher when the user has successfully requests a reward
 * @param placement - The TJPlacement that triggered the action request
 * @param request   - The TJActionRequest object
 * @param itemId    - The itemId for whose reward has been requested
 * @param quantity  - The quantity of the reward for the requested itemId
 */

- (void)placement:(TJPlacement *)placement
 didRequestReward:(nullable TJActionRequest *)request
		   itemId:(nullable NSString *)itemId
		 quantity:(int)quantity;

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
	virtual void InitIronSource(const FString& UserId, bool bOfferwallEnable) override;
	virtual void ForceUpdateIronSourceUser(const FString& UserId) override;
	virtual void SetSegmentInfo(const FString& SegmentName, const FString& SegmentRevenueKey, float SegmentRevenue) override;
	virtual bool HasRewardedVideo() const override;
	virtual FString GetPlacementRewardName(const FString& PlacementName) const override;
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const override;
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const override;
	virtual void ShowRewardedVideo(const FString& PlacementName) const override;
	virtual void SetConsent(bool bConsent) const override;
	virtual void SetOfferwallConsent(const FPsOfferwallPrivacyPolicy& OfferwallPP) const override;

	virtual void LoadInterstitial() override;
	virtual bool IsInterstitialReady() const override;
	virtual void ShowInterstitial(const FString& PlacementName) const override;
	virtual bool IsInterstitialCappedForPlacement(const FString& PlacementName) const override;

	virtual bool HasOfferwall() const override;
	virtual void ShowOfferwall() const override;
	// End UPsIronSourceProxy interface

	void InitOfferwall();
	void OnTapjoyConnected(bool bSuccess);

private:
	PSISDelegate* Delegate;
	PSISLogDelegate* LogDelegate;
	PSISImpressionDataDelegate* ImpressionDelegate;
	PSISInterstitialDelegate* InterstitialDelegate;
	PSTapjoyConnectionDelegate* TapjoyConnectionDelegate;
	TJPlacement* TapjoyPlacement;
	PSTapjoyPlacementDelegate* TapjoyPlacementDelegate;
	bool bTapjoyInitialized;

#endif // WITH_IRONSOURCE && PLATFORM_IOS
};
