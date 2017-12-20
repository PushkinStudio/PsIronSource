// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "IronSourceProxy.generated.h"

UCLASS()
class PSIRONSOURCE_API UIronSourceProxy : public UObject
{
	GENERATED_UCLASS_BODY()

	/////////////////////////////////////////////////////////////////////////
	// Setup

	/** Initialize */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void InitIronSource(const FString& UserId);

	/** Whether SDK is initialized */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	bool IsInitialized() const;

	/** Whether there's rewarded video available */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool HasRewardedVideo() const;

	/** Get reward name for placement */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual FString GetPlacementRewardName(const FString& PlacementName) const;

	/** Get reward amount for placement */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual FString GetPlacementRewardAmount(const FString& PlacementName) const;

	/** Whether rewarded video capped for placement */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual bool IsRewardedVideoCappedForPlacement(const FString& PlacementName) const;

	/** Show rewarded video */
	UFUNCTION(BlueprintCallable, Category = "IronSource")
	virtual void ShowRewardedVideo(const FString& PlacementName) const;

protected:
	/** Whether SDK is initialized */
	bool bIronSourceInitialized;
};
