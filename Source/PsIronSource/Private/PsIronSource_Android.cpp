// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#include "PsIronSource_Android.h"

UPsIronSource_Android::UPsIronSource_Android(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_IRONSOURCE && PLATFORM_ANDROID
	bTapjoyInitialized = false;
#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
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
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_SetSegmentInfo;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_setConsent;

jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_loadInterstitial;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialReady;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showInterstitial;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_isInterstitialCappedForPlacement;

jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_hasOfferwall;
jmethodID UPsIronSource_Android::AndroidThunkJava_IronSource_showOfferwall;

jmethodID UPsIronSource_Android::AndroidThunkJava_Tapjoy_init;
jmethodID UPsIronSource_Android::AndroidThunkJava_Tapjoy_request_content;
jmethodID UPsIronSource_Android::AndroidThunkJava_Tapjoy_setConsent;

UPsIronSourceProxy* ISProxy;
UPsIronSource_Android* ISAndroid;

void UPsIronSource_Android::InitIronSource(const FString& UserId, bool bOfferwallEnable)
{
	if (bOfferwallEnable)
	{
		InitOfferwall();
	}
	
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

		UPsIronSource_Android::AndroidThunkJava_IronSource_init = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_init", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring JUserId = Env->NewStringUTF(TCHAR_TO_UTF8(*UserId));
		jstring JAppKey = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->IronSourceAndroidAppKey)));
		jstring JAPSAppId = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->APSAndroidAppId)));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_init, JAppKey, JUserId, JAPSAppId);
		Env->DeleteLocalRef(JUserId);
		Env->DeleteLocalRef(JAppKey);
		Env->DeleteLocalRef(JAPSAppId);
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

void UPsIronSource_Android::SetSegmentInfo(const FString& SegmentName, const FString& SegmentRevenueKey, float SegmentRevenue)
{
	LOGD("%s: Update IronSource SegmentName", TCHAR_TO_ANSI(*PS_FUNC_LINE));

	if (!bIronSourceInitialized)
	{
		LOGD("%s: Trying to update ironsource segment name when it's not yet initialized!", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		return;
	}

	if (SegmentName.IsEmpty())
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment name but it's empty!"), *PS_FUNC_LINE);
		return;
	}

	if (SegmentRevenueKey.IsEmpty())
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment revenue but field key is empty!"), *PS_FUNC_LINE);
		return;
	}

	if (FMath::IsNegativeFloat(SegmentRevenue))
	{
		UE_LOG(LogPsIronSource, Error, TEXT("%s: Trying to update IronSource segment revenue but it's negative '%f'!"), *PS_FUNC_LINE, SegmentRevenue);
		return;
	}

	const FString SegmentRevenueString = FString::SanitizeFloat(SegmentRevenue);
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_SetSegmentInfo = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_SetSegmentInfo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", false);

		jstring JSegmentName = Env->NewStringUTF(TCHAR_TO_UTF8(*SegmentName));
		jstring JSegmentRevenueKey = Env->NewStringUTF(TCHAR_TO_UTF8(*SegmentRevenueKey));
		jstring JSegmentRevenue = Env->NewStringUTF(TCHAR_TO_UTF8(*SegmentRevenueString));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_SetSegmentInfo, JSegmentName, JSegmentRevenueKey, JSegmentRevenue);
		Env->DeleteLocalRef(JSegmentName);
		Env->DeleteLocalRef(JSegmentRevenueKey);
		Env->DeleteLocalRef(JSegmentRevenue);
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

void UPsIronSource_Android::SetConsent(bool bConsent) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		UPsIronSource_Android::AndroidThunkJava_IronSource_setConsent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_setConsent", "(Z)V", false);

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_IronSource_setConsent, bConsent);
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::SetOfferwallConsent(const FPsOfferwallPrivacyPolicy& OfferwallPP) const
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		jstring JCCPASetting = Env->NewStringUTF(TCHAR_TO_UTF8(*OfferwallPP.CCPASetting));
		bool bSubjectToGDPR = OfferwallPP.bSubjectToGDPR;
		bool bUserConsent = OfferwallPP.bUserConsent;

		UPsIronSource_Android::AndroidThunkJava_Tapjoy_setConsent = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Tapjoy_setConsent", "(Ljava/lang/String;ZZ)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_Tapjoy_setConsent, JCCPASetting, bSubjectToGDPR, bUserConsent);

		Env->DeleteLocalRef(JCCPASetting);
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

void UPsIronSource_Android::InitOfferwall()
{
	LOGD("%s", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		LOGD("%s: Initialize Tapjoy with Android SDK", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		if (bTapjoyInitialized)
		{
			LOGD("%s: Trying to initialize Tapjoy when it's already been initialized!", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			return;
		}

		ISProxy = this;
		ISAndroid = this;

		if (GetDefault<UPsIronSourceSettings>()->TapjoyAPIKeyAndroid.IsEmpty() == false)
		{
			bTapjoyInitialized = true;

			jstring JTapjoyAPIKeyAndroid = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->TapjoyAPIKeyAndroid)));

			UPsIronSource_Android::AndroidThunkJava_Tapjoy_init = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Tapjoy_init", "(Ljava/lang/String;)V", false);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_Tapjoy_init, JTapjoyAPIKeyAndroid);

			Env->DeleteLocalRef(JTapjoyAPIKeyAndroid);
		}
		else
		{
			UE_LOG(LogPsIronSource, Error, TEXT("%s: no TapjoyAPIKeyAndroid provided"), *PS_FUNC_LINE);
		}
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

void UPsIronSource_Android::OnTapjoyConnected(bool bSuccess)
{
	if (bSuccess)
	{
		OnOfferwallConnected.Broadcast();
	}

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		if (bSuccess)
		{
			if (GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement.IsEmpty() == false)
			{
				jstring JTapjoyOfferwallPlacement = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement)));

				UPsIronSource_Android::AndroidThunkJava_Tapjoy_request_content = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Tapjoy_request_content", "(Ljava/lang/String;)V", false);
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_Tapjoy_request_content, JTapjoyOfferwallPlacement);

				Env->DeleteLocalRef(JTapjoyOfferwallPlacement);
			}
			else
			{
				LOGD("%s: no TapjoyOfferwallPlacement provided", TCHAR_TO_ANSI(*PS_FUNC_LINE));
				bTapjoyInitialized = false;
			}
		}
		else
		{
			LOGD("%s: Error establishing connection with Tapjoy!", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			bTapjoyInitialized = false;
		}
	}
	else
	{
		LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*PS_FUNC_LINE));
	}
}

// The Rewarded Video ad view has opened. Your activity will loose focus
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdOpenedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

// The Rewarded Video ad view is about to be closed. Your activity will regain its focus
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdClosedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

// Indicates that there's an available ad.
// The adInfo object includes information about the ad that was loaded successfully
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdAvailableThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
{
	jclass DoubleClass = jenv->FindClass("java/lang/Double");
	jmethodID DoubleValueMethodId = jenv->GetMethodID(DoubleClass, "doubleValue", "()D");

	jclass AdInfoClass = jenv->FindClass("com/ironsource/mediationsdk/adunit/adapter/utility/AdInfo");
	jmethodID getAuctionIdMethodId = jenv->GetMethodID(AdInfoClass, "getAuctionId", "()Ljava/lang/String;");
	jmethodID getAdUnitMethodId = jenv->GetMethodID(AdInfoClass, "getAdUnit", "()Ljava/lang/String;");
	jmethodID getAdNetworkMethodId = jenv->GetMethodID(AdInfoClass, "getAdNetwork", "()Ljava/lang/String;");
	jmethodID getInstanceNameMethodId = jenv->GetMethodID(AdInfoClass, "getInstanceName", "()Ljava/lang/String;");
	jmethodID getInstanceIdMethodId = jenv->GetMethodID(AdInfoClass, "getInstanceId", "()Ljava/lang/String;");
	jmethodID getCountryMethodId = jenv->GetMethodID(AdInfoClass, "getCountry", "()Ljava/lang/String;");
	jmethodID getRevenueMethodId = jenv->GetMethodID(AdInfoClass, "getRevenue", "()Ljava/lang/Double;");
	jmethodID getPrecisionMethodId = jenv->GetMethodID(AdInfoClass, "getPrecision", "()Ljava/lang/String;");
	jmethodID getAbMethodId = jenv->GetMethodID(AdInfoClass, "getAb", "()Ljava/lang/String;");
	jmethodID getSegmentNameMethodId = jenv->GetMethodID(AdInfoClass, "getSegmentName", "()Ljava/lang/String;");
	jmethodID getLifetimeRevenueMethodId = jenv->GetMethodID(AdInfoClass, "getLifetimeRevenue", "()Ljava/lang/Double;");
	jmethodID getEncryptedCPMMethodId = jenv->GetMethodID(AdInfoClass, "getEncryptedCPM", "()Ljava/lang/String;");

	FPsIronSourceAdInfo Data;
	Data.AuctionId = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getAuctionIdMethodId));
	Data.AdUnit = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getAdUnitMethodId));
	Data.AdNetwork = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getAdNetworkMethodId));
	Data.InstanceName = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getInstanceNameMethodId));
	Data.InstanceId = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getInstanceIdMethodId));
	Data.Country = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getCountryMethodId));

	jobject RevenueDoubleObject = jenv->CallObjectMethod(jadInfo, getRevenueMethodId);
	if (RevenueDoubleObject != NULL)
	{
		Data.Revenue = jenv->CallDoubleMethod(RevenueDoubleObject, DoubleValueMethodId);
		jenv->DeleteLocalRef(RevenueDoubleObject);
	}

	Data.Precision = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getPrecisionMethodId));
	Data.Ab = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getAbMethodId));
	Data.SegmentName = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getSegmentNameMethodId));

	jobject LifetimeRevenueDoubleObject = jenv->CallObjectMethod(jadInfo, getLifetimeRevenueMethodId);
	if (LifetimeRevenueDoubleObject != NULL)
	{
		Data.LifetimeRevenue = jenv->CallDoubleMethod(LifetimeRevenueDoubleObject, DoubleValueMethodId);
		jenv->DeleteLocalRef(LifetimeRevenueDoubleObject);
	}

	Data.EncryptedCpm = FJavaHelper::FStringFromLocalRef(jenv, (jstring)jenv->CallObjectMethod(jadInfo, getEncryptedCPMMethodId));
	Data.bInitialized = true;

	jenv->DeleteLocalRef(DoubleClass);
	jenv->DeleteLocalRef(AdInfoClass);

	AsyncTask(ENamedThreads::GameThread, [Data]() {
		if (ISProxy != nullptr)
		{
			ISProxy->SetAdInfo(Data);
			ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoAvailable);
		}
		else
		{
			LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		}
	});
}

// Indicates that no ads are available to be displayed
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdUnavailableThunkCpp(JNIEnv* jenv, jobject thiz)
{
	AsyncTask(ENamedThreads::GameThread, []() {
		if (ISProxy != nullptr)
		{
			ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoNotAvailable);
		}
		else
		{
			LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		}
	});
}

// The user completed to watch the video, and should be rewarded.
// The placement parameter will include the reward data.
// When using server-to-server callbacks, you may ignore this event and wait for the ironSource server callback
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdRewardedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jplacement, jobject jadInfo)
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

// The rewarded video ad was failed to show
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onLevelPlayAdClickedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jplacement, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdReadyThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdOpenedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdClosedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdClickedThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onInterstitialAdShowSucceededThunkCpp(JNIEnv* jenv, jobject thiz, jobject jadInfo)
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

// Invoked when tapjoy connection is established
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onTapjoyConnectedThunkCpp(JNIEnv* jenv, jboolean success)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, [success]() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISAndroid->OnTapjoyConnected(success);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onTapjoyOfferwallContentShowThunkCpp(JNIEnv* jenv)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Opened, TEXT(""), 0);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onTapjoyOfferwallContentDismissThunkCpp(JNIEnv* jenv)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		AsyncTask(ENamedThreads::GameThread, []() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Closed, TEXT(""), 0);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});

		if (GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement.IsEmpty() == false)
		{
			jstring TapjoyOfferwallPlacement = jenv->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UPsIronSourceSettings>()->TapjoyOfferwallPlacement)));

			UPsIronSource_Android::AndroidThunkJava_Tapjoy_request_content = FJavaWrapper::FindMethod(jenv, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Tapjoy_request_content", "(Ljava/lang/String;)V", false);
			FJavaWrapper::CallVoidMethod(jenv, FJavaWrapper::GameActivityThis, UPsIronSource_Android::AndroidThunkJava_Tapjoy_request_content, TapjoyOfferwallPlacement);

			jenv->DeleteLocalRef(TapjoyOfferwallPlacement);
		}
		else
		{
			LOGD("%s: no TapjoyOfferwallPlacement provided", TCHAR_TO_ANSI(*PS_FUNC_LINE));
		}
	}
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_onTapjoyOfferwallRewardRequestThunkCpp(JNIEnv* jenv, jstring itemId, jint quantity)
{
	if (ISProxy != nullptr)
	{
		ISProxy->EnqueueEvent();

		const FString ItemId = FJavaHelper::FStringFromParam(jenv, itemId);
		const int32 Count = static_cast<int32>(quantity);

		AsyncTask(ENamedThreads::GameThread, [ItemId, Count]() {
			if (ISProxy != nullptr)
			{
				ISProxy->DequeueEvent();
				ISProxy->OfferwallStateDelegate.Broadcast(EIronSourceOfferwallEventType::Rewarded, ItemId, Count);
			}
			else
			{
				LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*PS_FUNC_LINE));
			}
		});
	}
}

#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
