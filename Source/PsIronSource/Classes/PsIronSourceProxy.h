// Copyright 2015-2021 Mail.Ru Group. All Rights Reserved.

#pragma once

#include <atomic>

#include "PsIronSourceProxy.generated.h"

UENUM(BlueprintType)
enum class EIronSourceEventType : uint8
{
	ReceivedReward,  // after the user has been rewarded
	VideoShowFailed, // there is a problem playing the video
	VideoOpened,     // when we take control, but before the video has started playing
	VideoClosed,     // when we return control back to your hands
	VideoStarted,    // video has started playing
	VideoEnded,      // video has stopped playing
	VideoTapped,     // video has been tapped
	VideoAvailable,  // video has changed availability to Available
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPSIronSourceVideoDelegate, EIronSourceEventType, Event);

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

	/** Set GDPR consent status */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void SetGDPRConsent(bool bConsent) const;

	/** Whether there are events waiting to be sent to game thread */
	bool HasQueuedEvents() const;

	/** Increment event counter */
	void EnqueueEvent();

	/** Decrement event counter */
	void DequeueEvent();

public:
	/** Delegate broadcasting video-related events */
	UPROPERTY(BlueprintAssignable)
	FPSIronSourceVideoDelegate VideoStateDelegate;

protected:
	/** Whether SDK is initialized */
	bool bIronSourceInitialized;

	/** Number of queued events */
	std::atomic<int32> QueuedEventsCount;
};
