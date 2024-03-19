// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#pragma once

#include <atomic>

#include "PsIronSourceProxy.generated.h"

UENUM(BlueprintType)
enum class EIronSourceEventType : uint8
{
	ReceivedReward,            // after the user has been rewarded
	VideoShowFailed,           // there is a problem playing the video
	VideoOpened,               // when we take control, but before the video has started playing
	VideoClosed,               // when we return control back to your hands
	VideoTapped,               // video has been tapped
	VideoAvailable,            // video has changed availability to Available
	VideoNotAvailable,         // video has changed availability to Not Available
	Impression,                // ad impression delegate callback
	InterstitialReady,         // when Interstitial Ad is ready to be shown after load function was called
	InterstitialLoadFailed,    // when there is no Interstitial Ad available after calling load function
	InterstitialOpened,        // when the Interstitial Ad Unit is opened
	InterstitialClosed,        // when the ad is closed and the user is about to return to the application
	InterstitialShowFailed,    // when Interstitial ad failed to show
	InterstitialShowSucceeded, // right before the Interstitial screen is about to open.
							   // NOTE - This event is available only for some of the networks.
							   // You should NOT treat this event as an interstitial impression, but rather use InterstitialAdOpenedEvent
	InterstitialClicked        // when the end user clicked on the interstitial ad, for supported networks only
};

UENUM(BlueprintType)
enum class EIronSourceOfferwallEventType : uint8
{
	Available,
	Opened,
	Closed,
	Rewarded,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSIronSourceVideoDelegate, EIronSourceEventType, Event);
DECLARE_MULTICAST_DELEGATE(FPSIronSourceOfferwallConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPSIronSourceOfferwallDelegate, EIronSourceOfferwallEventType, Event, FString, ItemId, int32, Count);

USTRUCT()
struct FPsIronSourceAdInfoCommon
{
	GENERATED_BODY()

	UPROPERTY()
	FString AuctionId;

	UPROPERTY()
	FString AdUnit;

	UPROPERTY()
	FString AdNetwork;

	UPROPERTY()
	FString InstanceName;

	UPROPERTY()
	FString InstanceId;

	UPROPERTY()
	FString Country;

	UPROPERTY()
	float Revenue;

	UPROPERTY()
	FString Precision;

	UPROPERTY()
	FString Ab;

	UPROPERTY()
	FString SegmentName;

	UPROPERTY()
	float LifetimeRevenue;

	UPROPERTY()
	FString EncryptedCpm;

	FPsIronSourceAdInfoCommon()
		: Revenue(0.f)
		, LifetimeRevenue(0.f)
	{
	}
};

USTRUCT()
struct FPsIronSourceImpressionData : public FPsIronSourceAdInfoCommon
{
	GENERATED_BODY()

	UPROPERTY()
	FString Placement;

	UPROPERTY()
	float ConversionValue;

	FPsIronSourceImpressionData()
		: ConversionValue(0.f)
	{
	}
};

USTRUCT()
struct FPsIronSourceAdInfo : public FPsIronSourceAdInfoCommon
{
	GENERATED_BODY()

	UPROPERTY()
	bool bInitialized;

	FPsIronSourceAdInfo()
		: bInitialized(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FPsOfferwallPrivacyPolicy 
{
	GENERATED_BODY()

	UPROPERTY()
	bool bOfferwallEnable;

	UPROPERTY()
	bool bSubjectToGDPR;

	UPROPERTY()
	bool bUserConsent;

	UPROPERTY()
	FString CCPASetting;

	FPsOfferwallPrivacyPolicy()
		: bOfferwallEnable(false)
		, bSubjectToGDPR(false)
		, bUserConsent(false)
	{
	}

	FPsOfferwallPrivacyPolicy(bool bInOfferwallEnable, bool bInSubjectToGDPR, bool bInGDPRUserConsent, const FString& InCCPASetting)
		: bOfferwallEnable(bInOfferwallEnable)
		, bSubjectToGDPR(bInSubjectToGDPR)
		, bUserConsent(bInGDPRUserConsent)
		, CCPASetting(InCCPASetting)
	{
	}
};

UCLASS()
class PSIRONSOURCE_API UPsIronSourceProxy : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/////////////////////////////////////////////////////////////////////////
	// Setup

	/** Set offerwall use client-side callbacks
	 *
	 * Call before SDK initialization
	 *
	 */
	virtual void SetOfferwallUseClientSideCallbacks(bool bValue);

	/** Initialize SDK */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void InitIronSource(const FString& UserId, bool bOfferwallEnable);

	/** Update userid after change account */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ForceUpdateIronSourceUser(const FString& UserId);

	/** Update segment info */
	virtual void SetSegmentInfo(const FString& SegmentName, const FString& SegmentRevenueKey, float SegmentRevenue);

	/** Whether SDK is initialized */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	bool IsInitialized() const;

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

	/** Set consent status
	 *
	 * Call before SDK initialization
	 *
	 */
	virtual void SetConsent(bool bConsent) const;

	/** Set Tapjoy offerwall consent
	 *
	 * Call only after SDK initialization and receiving OnOfferwallConnected callback
	 *
	 */
	virtual void SetOfferwallConsent(const FPsOfferwallPrivacyPolicy& OfferwallPP) const;

	/** Whether there are events waiting to be sent to game thread */
	bool HasQueuedEvents() const;

	/** Increment event counter */
	void EnqueueEvent();

	/** Decrement event counter */
	void DequeueEvent();

	/** Set ad info */
	void SetAdInfo(const FPsIronSourceAdInfo& InAdInfo);

	/** Get last ad info */
	FPsIronSourceAdInfo GetAdInfo() const;

	/** Set impression data */
	void SetImpressionData(const FPsIronSourceImpressionData& InImpressionData);

	/** Get last impression data */
	FPsIronSourceImpressionData GetImpressionData() const;

	/** Load interstitial to local cache on mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void LoadInterstitial();

	/** Check if interstitial loaded to local cache on mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool IsInterstitialReady() const;

	/** Show interstitial */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ShowInterstitial(const FString& PlacementName) const;

	/** Verify if a certain placement has reached its ad limit */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool IsInterstitialCappedForPlacement(const FString& PlacementName) const;

	/** Determine if a locally cached rewarded video exists on the mediation level */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool HasOfferwall() const;

	/** Show offerwall */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ShowOfferwall() const;

public:
	/** Delegate broadcasting video-related events */
	UPROPERTY(BlueprintAssignable)
	FPSIronSourceVideoDelegate VideoStateDelegate;

	/** Delegate broadcasting offerwall connection established */
	FPSIronSourceOfferwallConnected OnOfferwallConnected;

	/** Delegate broadcasting offerwall-related events */
	UPROPERTY(BlueprintAssignable)
	FPSIronSourceOfferwallDelegate OfferwallStateDelegate;

protected:
	/** Whether SDK is initialized */
	bool bIronSourceInitialized;

	/** Number of queued events */
	std::atomic<int32> QueuedEventsCount;

	/** Ad info instance */
	FPsIronSourceAdInfo AdInfo;

	/** Impression data instance */
	FPsIronSourceImpressionData ImpressionData;
};
