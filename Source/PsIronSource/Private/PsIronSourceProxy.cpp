// Copyright 2015-2021 Mail.Ru Group. All Rights Reserved.

#include "PsIronSourceProxy.h"

#include "PsIronSourceDefines.h"

UPsIronSourceProxy::UPsIronSourceProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIronSourceInitialized = false;
	QueuedEventsCount = 0;
}

/////////////////////////////////////////////////////////////////////////
// Setup

void UPsIronSourceProxy::InitIronSource(const FString& UserId)
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
}

void UPsIronSourceProxy::ForceUpdateIronSourceUser(const FString& UserId)
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
}

bool UPsIronSourceProxy::IsInitialized() const
{
	return bIronSourceInitialized;
}

bool UPsIronSourceProxy::HasRewardedVideo() const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
	return true;
}

FString UPsIronSourceProxy::GetPlacementRewardName(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
	return FString();
}

FString UPsIronSourceProxy::GetPlacementRewardAmount(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
	return FString();
}

bool UPsIronSourceProxy::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
	return false;
}

void UPsIronSourceProxy::ShowRewardedVideo(const FString& PlacementName) const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
	VideoStateDelegate.Broadcast(EIronSourceEventType::VideoShowFailed);
}

void UPsIronSourceProxy::SetGDPRConsent(bool bConsent) const
{
	UE_LOG(LogPsIronSource, Warning, TEXT("%s: Null proxy used"), *PS_FUNC_LINE);
}

bool UPsIronSourceProxy::HasQueuedEvents() const
{
	return QueuedEventsCount > 0;
}

void UPsIronSourceProxy::EnqueueEvent()
{
	QueuedEventsCount += 1;
	UE_LOG(LogPsIronSource, Log, TEXT("%s: QueuedEventsCount %d"), *PS_FUNC_LINE, QueuedEventsCount.load());
}

void UPsIronSourceProxy::DequeueEvent()
{
	QueuedEventsCount -= 1;
	UE_LOG(LogPsIronSource, Log, TEXT("%s: QueuedEventsCount %d"), *PS_FUNC_LINE, QueuedEventsCount.load());
}

void UPsIronSourceProxy::SetImpressionData(const FPsIronSourceImpressionData& InImpressionData)
{
	ImpressionData = InImpressionData;
}

FPsIronSourceImpressionData UPsIronSourceProxy::GetImpressionData() const
{
	return ImpressionData;
}
