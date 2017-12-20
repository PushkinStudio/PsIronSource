// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "IronSourceSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UIronSourceSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/** IronSource iOS app Key */
	UPROPERTY(Config, EditAnywhere)
	FString IronSourceIOSAppKey;

	/** IronSource Android app Key */
	UPROPERTY(Config, EditAnywhere)
	FString IronSourceAndroidAppKey;
};
