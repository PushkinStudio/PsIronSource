// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

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

	/** APS app id (iOS) */
	UPROPERTY(Config, EditAnywhere)
	FString APSIOSAppId;

	/** APS app id (Android) */
	UPROPERTY(Config, EditAnywhere)
	FString APSAndroidAppId;

	/** Tapjoy API key (iOS) */
	UPROPERTY(Config, EditAnywhere)
	FString TapjoyAPIKeyIOS;

	/** Tapjoy API key (Android) */
	UPROPERTY(Config, EditAnywhere)
	FString TapjoyAPIKeyAndroid;

	/** Tapjoy offerwall placement name */
	UPROPERTY(Config, EditAnywhere)
	FString TapjoyOfferwallPlacement;
};
