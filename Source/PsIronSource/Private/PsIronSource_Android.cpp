// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#include "PsIronSource_Android.h"

UPsIronSource_Android::UPsIronSource_Android(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_IRONSOURCE && PLATFORM_ANDROID

#include "Android/AndroidApplication.h"
#include "PsIronSourceDefines.h"
#include "PsIronSourceSettings.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "[CPP] PSIronSource"

jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_init;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_ForceUpdateUser;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_setGDPRConsent;

jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_loadInterstitial;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialReady;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showInterstitial;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialCappedForPlacement;

jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_hasOfferwall;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwall;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwallWithPlacement;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_getOfferwallCredits;

UPsIronSourceProxy* ISProxy;

void UPsIronSource_Android::InitIronSource(const FString& UserId)
{
	LOGD("%s: Initialize IronSource with Android SDK", TCHAR_TO_ANSI(*PS_FUNC_LINE));

	if (bIronSourceInitialized)
	{
		LOGD("%s: Trying to initialize IronSource when it's already been initialized!", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return;
	}

	bIronSourceInitialized = true;

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		ISProxy = this;

		UPsIronSource_Android::AndroidThunkJava_IronSource_init = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_init", "(Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring JUserId = Env->NewStringUTF(TCHAR_TO_UTF8(*UserId));
		jstring JAppKey = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->IronSourceAndroidAppKey)));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_init, JAppKey, JUserId);
		Env->DeleteLocalRef(JUserId);
		Env->DeleteLocalRef(JAppKey);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::ForceUpdateIronSourceUser(const FString& UserId)
{
	LOGD("%s: Update IronSource UserId", TCHAR_TO_ANSI(*PS_FUNC_LINE));

	if (!bIronSourceInitialized)
	{
		LOGD("%s: Trying to update ironsource userid when it's not yet initialized!", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return;
	}

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_ForceUpdateUser = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_ForceUpdateUser", "(Ljava/lang/String;)V", false);

		jstring JUserId = Env->NewStringUTF(TCHAR_TO_UTF8(*UserId));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_ForceUpdateUser, JUserId);
		Env->DeleteLocalRef(JUserId);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

bool UPsIronSource_Android::HasRewardedVideo() const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_hasRewardedVideo", "()Z", false);
		const bool bHasVideo = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo);
		return bHasVideo;
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return false;
	}
}

FString UPsIronSource_Android::GetPlacementRewardName(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_getPlacementRewardName", "(Ljava/lang/String;)Ljava/lang/String;", false);
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		jstring Name = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName, JPlacementName);
		Env->DeleteLocalRef(JPlacementName);
		const char* nativeName = Env->GetStringUTFChars(Name, 0);
		return FString(nativeName);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return FString();
	}
}

FString UPsIronSource_Android::GetPlacementRewardAmount(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_getPlacementRewardAmount", "(Ljava/lang/String;)I", false);
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		const int32 Amount = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount, JPlacementName);
		Env->DeleteLocalRef(JPlacementName);
		return FString::FromInt(Amount);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return FString();
	}
}

bool UPsIronSource_Android::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement", "(Ljava/lang/String;)Z", false);
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		const bool bCapped = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement, JPlacementName);
		Env->DeleteLocalRef(JPlacementName);
		return bCapped;
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return false;
	}
}

void UPsIronSource_Android::ShowRewardedVideo(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_showRewardedVideo", "(Ljava/lang/String;)V", false);

		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo, JPlacementName);
		Env->DeleteLocalRef(JPlacementName);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::SetGDPRConsent(bool bConsent) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_setGDPRConsent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_setGDPRConsent", "(Z)V", false);

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_setGDPRConsent, bConsent);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::LoadInterstitial()
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_loadInterstitial = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_loadInterstitial", "()V", false);

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_loadInterstitial);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

bool UPsIronSource_Android::IsInterstitialReady() const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialReady = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_isInterstitialReady", "()Z", false);

		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialReady);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return false;
	}
}

void UPsIronSource_Android::ShowInterstitial(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_showInterstitial = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_showInterstitial", "(Ljava/lang/String;)V", false);
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_showInterstitial, JPlacementName);

		Env->DeleteLocalRef(JPlacementName);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

bool UPsIronSource_Android::IsInterstitialCappedForPlacement(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialCappedForPlacement = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_isInterstitialCappedForPlacement", "(Ljava/lang/String;)Z", false);
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
		const bool bSuccess = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialCappedForPlacement, JPlacementName);

		Env->DeleteLocalRef(JPlacementName);
		return bSuccess;
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return false;
	}
}

bool UPsIronSource_Android::HasOfferwall() const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_hasOfferwall = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_hasOfferwall", "()Z", false);
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_hasOfferwall);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return false;
	}
}

void UPsIronSource_Android::ShowOfferwall() const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwall = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_showOfferwall", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwall);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::ShowOfferwallWithPlacement(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));

		UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwallWithPlacement = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_showOfferwallWithPlacement", "(Ljava/lang/String;)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwallWithPlacement, JPlacementName);

		Env->DeleteLocalRef(JPlacementName);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::GetOfferwallCredits() const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_getOfferwallCredits = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_getOfferwallCredits", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_getOfferwallCredits);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdOpenedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoOpened);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdClosedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoClosed);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when there is a change in the ad availability status
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAvailabilityChangedThunkCpp(JNIEnv* jenv, jobject thiz, jboolean available)
{
	// this event does not need to be counted
	AsyncTask(ENamedThreads::GameThread, [available]() {
		if (ISProxy != nullptr)
		{
			if (available)
			{
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoAvailable);
			}
			else
			{
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoNotAvailable);
			}
		}
		else
		{
			LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		}
	});
}

// Invoked when the RewardedVideo ad view has opened
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdStartedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoStarted);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when the RewardedVideo ad view is about to be closed
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdEndedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoEnded);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when the user completed the video and should be rewarded
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdRewardedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::ReceivedReward);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when RewardedVideo call to show a rewarded video has failed
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoShowFailed);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onRewardedVideoAdClickedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoTapped);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onImpressionSuccessThunkCpp(JNIEnv* jenv, jobject thiz, jobject jImpressionData)
{
	if (ISProxy != nullptr)
	{
		jclass DoubleClass = jenv->FindClass("java/lang/Double");
		jmethodID DoubleValueMethodId = jenv->GetMethodID(DoubleClass, "doubleValue", "()D");

		jclass ImpressionDataClass = jenv->FindClass("com/ironsource/mediationsdk/impressionData/ImpressionData");
		jmethodID getAuctionIdMethodId = jenv->GetMethodID(ImpressionDataClass, "getAuctionId", "()Ljava/lang/String;");
		jmethodID getAdUnitMethodId = jenv->GetMethodID(ImpressionDataClass, "getAdUnit", "()Ljava/lang/String;");
		jmethodID getCountryMethodId = jenv->GetMethodID(ImpressionDataClass, "getCountry", "()Ljava/lang/String;");
		jmethodID getAbMethodId = jenv->GetMethodID(ImpressionDataClass, "getAb", "()Ljava/lang/String;");
		jmethodID getSegmentNameMethodId = jenv->GetMethodID(ImpressionDataClass, "getSegmentName", "()Ljava/lang/String;");
		jmethodID getPlacementMethodId = jenv->GetMethodID(ImpressionDataClass, "getPlacement", "()Ljava/lang/String;");
		jmethodID getAdNetworkMethodId = jenv->GetMethodID(ImpressionDataClass, "getAdNetwork", "()Ljava/lang/String;");
		jmethodID getInstanceNameMethodId = jenv->GetMethodID(ImpressionDataClass, "getInstanceName", "()Ljava/lang/String;");
		jmethodID getInstanceIdMethodId = jenv->GetMethodID(ImpressionDataClass, "getInstanceId", "()Ljava/lang/String;");
		jmethodID getRevenueMethodId = jenv->GetMethodID(ImpressionDataClass, "getRevenue", "()Ljava/lang/Double;");
		jmethodID getPrecisionMethodId = jenv->GetMethodID(ImpressionDataClass, "getPrecision", "()Ljava/lang/String;");
		jmethodID getLifetimeRevenueMethodId = jenv->GetMethodID(ImpressionDataClass, "getLifetimeRevenue", "()Ljava/lang/Double;");
		jmethodID getEncryptedCPMMethodId = jenv->GetMethodID(ImpressionDataClass, "getEncryptedCPM", "()Ljava/lang/String;");

		FPsIronSourceImpressionData Data;
		Data.AuctionId = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getAuctionIdMethodId));
		Data.AdUnit = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getAdUnitMethodId));
		Data.AdNetwork = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getAdNetworkMethodId));
		Data.InstanceName = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getInstanceNameMethodId));
		Data.InstanceId = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getInstanceIdMethodId));
		Data.Country = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getCountryMethodId));
		Data.Placement = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getPlacementMethodId));

		jobject RevenueDoubleObject = jenv->CallObjectMethod(jImpressionData, getRevenueMethodId);
		if (RevenueDoubleObject != NULL)
		{
			Data.Revenue = jenv->CallDoubleMethod(RevenueDoubleObject, DoubleValueMethodId);
			jenv->DeleteLocalRef(RevenueDoubleObject);
		}

		Data.Precision = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getPrecisionMethodId));
		Data.Ab = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getAbMethodId));
		Data.SegmentName = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getSegmentNameMethodId));

		jobject LifetimeRevenueDoubleObject = jenv->CallObjectMethod(jImpressionData, getLifetimeRevenueMethodId);
		if (LifetimeRevenueDoubleObject != NULL)
		{
			Data.LifetimeRevenue = jenv->CallDoubleMethod(LifetimeRevenueDoubleObject, DoubleValueMethodId);
			jenv->DeleteLocalRef(LifetimeRevenueDoubleObject);
		}

		Data.EncryptedCpm = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getEncryptedCPMMethodId));
		// no Data.ConversionValue

		jenv->DeleteLocalRef(DoubleClass);
		jenv->DeleteLocalRef(ImpressionDataClass);

		AsyncTask(ENamedThreads::GameThread, [Data]() {
			if (ISProxy != nullptr)
			{
				ISProxy->SetImpressionData(Data);
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::Impression);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdReadyThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialReady);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdLoadFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialLoadFailed);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdOpenedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialOpened);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdClosedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialClosed);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialShowFailed);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdClickedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialClicked);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdShowSucceededThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::InterstitialShowSucceeded);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when there is a change in the offerwall availability status
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onOfferwallAvailabilityChangedThunkCpp(JNIEnv* jenv, jobject thiz, jboolean available)
{
	// this event does not need to be counted
	AsyncTask(ENamedThreads::GameThread, [available]() {
		if (ISProxy != nullptr)
		{
			if (available)
			{
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Available, 0, 0, false);
			}
			else
			{
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::NotAvailable, 0, 0, false);
			}
		}
		else
		{
			LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		}
	});
}

// Invoked when offerwall successfully loads for the user, after calling the 'showOfferwall' method
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onOfferwallOpenedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Opened, 0, 0, false);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when offerwall fails to load for the user, after calling the 'showOfferwall' method
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onOfferwallShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::ShowFailed, 0, 0, false);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

/**
 * Invoked each time the user completes an Offer.
 * Award the user with the credit amount corresponding to the value of the *‘credits’ parameter.
 * @param credits - The number of credits the user has earned.
 * @param totalCredits - The total number of credits ever earned by the user.
 * @param totalCreditsFlag - In some cases, we won’t be able to provide the exact
 * amount of credits since the last event (specifically if the user clears
 * the app’s data). In this case the ‘credits’ will be equal to the ‘totalCredits’, and this flag will be ‘true’.
 * @return bool - true if you received the callback and rewarded the user, otherwise false.
 */
JNI_METHOD jboolean Java_com_epicgames_ue4_GameActivity_onOfferwallCreditedThunkCpp(JNIEnv* jenv, jobject thiz, jint credits, jint totalCredits, jboolean totalCreditsFlag)
{
	jboolean bIsUserRewarded = JNI_TRUE;

	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, [credits, totalCredits, totalCreditsFlag]() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Credited,
					static_cast<int32>(credits), static_cast<int32>(totalCredits), totalCreditsFlag == JNI_TRUE);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}

	return bIsUserRewarded;
}

// Invoked when the method 'getOfferwallCredits' fails to retrieve the user's credit balance info
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onOfferwallGetCreditsFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::GetCreditsFailed, 0, 0, false);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

// Invoked when the user is about to return to the application after closing
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onOfferwallClosedThunkCpp(JNIEnv* jenv, jobject thiz)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Closed, 0, 0, false);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
