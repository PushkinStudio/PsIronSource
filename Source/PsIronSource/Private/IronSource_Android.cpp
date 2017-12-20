// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"

#if WITH_IRONSOURCE && PLATFORM_ANDROID
	#include "Android/AndroidJNI.h"
	#include "Android/AndroidApplication.h"
	#include <android_native_app_glue.h>
#endif // WITH_IRONSOURCE && PLATFORM_ANDROID

UIronSource_Android::UIronSource_Android(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_IRONSOURCE && PLATFORM_ANDROID

void UIronSource_Android::InitIronSource(const FString& UserId)
{
	UE_LOG(LogIronSource, Warning, TEXT("%s: Initialize IronSource with Android SDK"), *VA_FUNC_LINE);

	if (bIronSourceInitialized)
	{
		UE_LOG(LogIronSource, Error, TEXT("%s: Trying to initialize IronSource when it's already been initialized!"), *VA_FUNC_LINE);
		return;
	}

	bIronSourceInitialized = true;
}

#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
