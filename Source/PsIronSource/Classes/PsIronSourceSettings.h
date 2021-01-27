// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#pragma once

#include "PsIronSourceSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UPsIronSourceSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/** Whether IronSource is enabled */
	UPROPERTY(Config, EditAnywhere)
	bool bIronSourceEnable;

	/** IronSource iOS app Key */
	UPROPERTY(Config, EditAnywhere)
	FString IronSourceIOSAppKey;

	/** AdMob app id (iOS) */
	UPROPERTY(Config, EditAnywhere)
	FString AdMobIOSAppId;

	/** IronSource Android app Key */
	UPROPERTY(Config, EditAnywhere)
	FString IronSourceAndroidAppKey;

	/** AdMob app id (Android) */
	UPROPERTY(Config, EditAnywhere)
	FString AdMobAndroidAppId;
};
