// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.generated.h"

UENUM(BlueprintType)
enum class EIronSourceVideoEventType : uint8
{
	ReceivedReward,  // after the user has been rewarded
	VideoShowFailed, // there is a problem playing the video
	VideoOpened,     // when we take control, but before the video has started playing
	VideoClosed,     // when we return control back to your hands
	VideoStarted,    // video has started playing
	VideoEnded,      // video has stopped playing
	VideoTapped,     // video has been tapped
};

UENUM(BlueprintType)
enum class EIronSourceInterstitialEventType : uint8
{
    InterstitialLoadFailed,	// No ad available after load
    InterstitialShowFailed,	// A problem with showing ad
    InterstitialOpened,		// Ad was opened
    InterstitialClosed,		// Ad was closed
    InterstitialSucceeded,	// Ad successfully shown on screen
    InterstitialTapped,		// Ad has been tapped
    InterstitialReady,		// Ad is loaded and ready to be shown
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSIronSourceVideoDelegate, EIronSourceVideoEventType, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSIronSourceInterstitialDelegate, EIronSourceInterstitialEventType, Event);

UCLASS()
class PSIRONSOURCE_API UPsIronSourceProxy : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/////////////////////////////////////////////////////////////////////////
	// Setup

	/** Initialize */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void InitIronSource(const FString& UserId);

	/** Update userid after change account */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ForceUpdateIronSourceUser(const FString& UserId);

	/** Whether SDK is initialized */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	bool IsInitialized() const;

	/** Set GDPR consent status */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
    virtual void SetGDPRConsent(bool bConsent) const;

	// Video
	
	/** Determine if a locally cached rewarded video exists on the mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool HasRewardedVideo() const;

	/** Get reward name for placement */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual FString GetPlacementRewardName(const FString& PlacementName) const;

	/** Get reward amount for placement */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const;

	/** Verify if a certain placement has reached its ad limit */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const;

	/** Show rewarded video */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ShowRewardedVideo(const FString& PlacementName) const;

	// Interstitial

	/** Request and cache Interstitial on the mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
    virtual void LoadInterstitial() const;
	
	/** Determine if a locally cached Interstitial ready on the mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
    virtual bool IsInterstitialReady() const;

	/** Verify if a certain placement has reached its ad limit */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
    virtual bool IsInterstitialCappedForPlacement(const FString& PlacementName) const;

	/** Show interstitial */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ShowInterstitial(const FString& PlacementName) const;

public:
	
	/** Delegate broadcasting video-related events */
	UPROPERTY(BlueprintAssignable, Category = "IronSource")
	FPSIronSourceVideoDelegate VideoStateDelegate;

	/** Delegate broadcasting interstitial-related events */
	UPROPERTY(BlueprintAssignable, Category = "IronSource")
    FPSIronSourceInterstitialDelegate InterstitialStateDelegate;

protected:
	
	/** Whether SDK is initialized */
	bool bIronSourceInitialized;
};
