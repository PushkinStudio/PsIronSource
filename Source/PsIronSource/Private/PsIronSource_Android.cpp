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
	AsyncTask(ENamedThreads::GameThread, []() {
		if (ISProxy != nullptr)
		{
			ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoAvailable);
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
		Data.Revenue = jenv->CallDoubleMethod(RevenueDoubleObject, DoubleValueMethodId);
		jenv->DeleteLocalRef(RevenueDoubleObject);

		Data.Precision = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getPrecisionMethodId));
		Data.Ab = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getAbMethodId));
		Data.SegmentName = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jImpressionData, getSegmentNameMethodId));

		jobject LifetimeRevenueDoubleObject = jenv->CallObjectMethod(jImpressionData, getLifetimeRevenueMethodId);
		Data.LifetimeRevenue = jenv->CallDoubleMethod(LifetimeRevenueDoubleObject, DoubleValueMethodId);
		jenv->DeleteLocalRef(LifetimeRevenueDoubleObject);

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

#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
