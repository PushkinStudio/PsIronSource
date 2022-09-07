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

public:
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
#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
};
