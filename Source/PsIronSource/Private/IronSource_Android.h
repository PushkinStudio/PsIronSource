// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "IronSourceProxy.h"
#include "IronSource_Android.generated.h"

UCLASS()
class UIronSource_Android : public UIronSourceProxy
{
	GENERATED_UCLASS_BODY()

#if WITH_IRONSOURCE && PLATFORM_ANDROID
	// Begin UIronSourceProxy interface
	virtual void InitIronSource(const FString& UserId) override;
	// End UIronSourceProxy interface
#endif // WITH_IRONSOURCE && PLATFORM_ANDROID

};
