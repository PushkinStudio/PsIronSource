// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"
#include "PsIronSource_Android.generated.h"

#if PLATFORM_ANDROID
#include <jni.h>
#endif // PLATFORM_ANDROID

UCLASS()
class UPsIronSource_Android : public UPsIronSourceProxy
{
	GENERATED_UCLASS_BODY()

#if WITH_IRONSOURCE && PLATFORM_ANDROID
	// Begin UPsIronSourceProxy interface
	virtual void SetOfferwallUseClientSideCallbacks (bool bValue) override;
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

	virtual bool HasOfferwall() const override;
	virtual void ShowOfferwall() const override;
	virtual void ShowOfferwallWithPlacement(const FString& PlacementName) const override;
	virtual void GetOfferwallCredits() const override;
	// End UPsIronSourceProxy interface

public:
	static jmethodID AndroidThunkJava_IronSource_setOfferwallUseClientSideCallbacks;
	static jmethodID AndroidThunkJava_IronSource_init;
	static jmethodID AndroidThunkJava_IronSource_ForceUpdateUser;
	static jmethodID AndroidThunkJava_IronSource_hasRewardedVideo;
	static jmethodID AndroidThunkJava_IronSource_getPlacementRewardName;
	static jmethodID AndroidThunkJava_IronSource_getPlacementRewardAmount;
	static jmethodID AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement;
	static jmethodID AndroidThunkJava_IronSource_showRewardedVideo;
	static jmethodID AndroidThunkJava_IronSource_setGDPRConsent;

	static jmethodID AndroidThunkJava_IronSource_loadInterstitial;
	static jmethodID AndroidThunkJava_IronSource_isInterstitialReady;
	static jmethodID AndroidThunkJava_IronSource_showInterstitial;
	static jmethodID AndroidThunkJava_IronSource_isInterstitialCappedForPlacement;

	static jmethodID AndroidThunkJava_IronSource_hasOfferwall;
	static jmethodID AndroidThunkJava_IronSource_showOfferwall;
	static jmethodID AndroidThunkJava_IronSource_showOfferwallWithPlacement;
	static jmethodID AndroidThunkJava_IronSource_getOfferwallCredits;
#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
};
