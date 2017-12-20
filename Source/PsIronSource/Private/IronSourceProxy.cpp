// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"
#include "IronSourceProxy.h"

UIronSourceProxy::UIronSourceProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIronSourceInitialized = false;
}

/////////////////////////////////////////////////////////////////////////
// Setup

void UIronSourceProxy::InitIronSource(const FString& UserId)
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
}

bool UIronSourceProxy::IsInitialized() const
{
	return bIronSourceInitialized;
}

bool UIronSourceProxy::HasRewardedVideo() const
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
	return false;
}

FString UIronSourceProxy::GetPlacementRewardName(const FString& PlacementName) const
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
	return FString();
}

FString UIronSourceProxy::GetPlacementRewardAmount(const FString& PlacementName) const
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
	return FString();
}

bool UIronSourceProxy::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
	return false;
}

void UIronSourceProxy::ShowRewardedVideo(const FString& PlacementName) const
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Null proxy used"), *VA_FUNC_LINE);
}
