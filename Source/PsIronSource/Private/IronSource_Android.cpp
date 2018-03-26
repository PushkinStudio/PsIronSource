// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"

UIronSource_Android::UIronSource_Android(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#if WITH_IRONSOURCE && PLATFORM_ANDROID

#include "AndroidApplication.h"
#include "Runtime/Launch/Public/Android/AndroidJNI.h"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOG_TAG    "[CPP] PSIronSource"

jmethodID UIronSource_Android::AndroidThunkJava_IronSource_init;
jmethodID UIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo;
jmethodID UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName;
jmethodID UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount;
jmethodID UIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement;
jmethodID UIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo;

UIronSourceProxy* ISProxy;

void UIronSource_Android::InitIronSource(const FString& UserId)
{
	LOGD("%s: Initialize IronSource with Android SDK", TCHAR_TO_ANSI(*VA_FUNC_LINE));

	if (bIronSourceInitialized)
	{
		LOGD("%s: Trying to initialize IronSource when it's already been initialized!", TCHAR_TO_ANSI(*VA_FUNC_LINE));
		return;
	}

	bIronSourceInitialized = true;

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        ISProxy = this;

        UIronSource_Android::AndroidThunkJava_IronSource_init = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_init", "(Ljava/lang/String;Ljava/lang/String;)V", false);

        jstring JUserId = Env->NewStringUTF(TCHAR_TO_UTF8(*UserId));
        jstring JAppKey = Env->NewStringUTF(TCHAR_TO_UTF8(*(GetDefault<UIronSourceSettings>()->IronSourceAndroidAppKey)));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_init, JAppKey, JUserId);
        Env->DeleteLocalRef(JUserId);
        Env->DeleteLocalRef(JAppKey);
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

bool UIronSource_Android::HasRewardedVideo() const
{
	LOGD("%s", TCHAR_TO_ANSI(*VA_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        UIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_hasRewardedVideo", "()Z", false);
        const bool bHasVideo = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_hasRewardedVideo);
        return bHasVideo;
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    	return false;
    }
}

FString UIronSource_Android::GetPlacementRewardName(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*VA_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_getPlacementRewardName", "(Ljava/lang/String;)Ljava/lang/String;", false);
        jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
        jstring Name = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardName, JPlacementName);
        Env->DeleteLocalRef(JPlacementName);
        const char *nativeName = Env->GetStringUTFChars(Name, 0);
        return FString(nativeName);
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    	return FString();
    }
}

FString UIronSource_Android::GetPlacementRewardAmount(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*VA_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_getPlacementRewardAmount", "(Ljava/lang/String;)I", false);
        jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
        const int32 Amount = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_getPlacementRewardAmount, JPlacementName);
        Env->DeleteLocalRef(JPlacementName);
        return FString::FromInt(Amount);
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    	return FString();
    }
}

bool UIronSource_Android::IsRewardedVideoCappedForPlacement(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*VA_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        UIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement", "(Ljava/lang/String;)Z", false);
        jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
        const bool bCapped = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_isRewardedVideoCappedForPlacement, JPlacementName);
        Env->DeleteLocalRef(JPlacementName);
        return bCapped;
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    	return false;
    }
}

void UIronSource_Android::ShowRewardedVideo(const FString& PlacementName) const
{
	LOGD("%s", TCHAR_TO_ANSI(*VA_FUNC_LINE));
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
    {
        UIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IronSource_showRewardedVideo", "(Ljava/lang/String;)V", false);

        jstring JPlacementName = Env->NewStringUTF(TCHAR_TO_UTF8(*PlacementName));
        FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, UIronSource_Android::AndroidThunkJava_IronSource_showRewardedVideo, JPlacementName);
        Env->DeleteLocalRef(JPlacementName);
    }
    else
    {
    	LOGD("%s: invalid JNIEnv", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdOpenedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoOpened);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdClosedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoClosed);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

// Invoked when there is a change in the ad availability status
JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAvailabilityChangedThunkCpp(JNIEnv* jenv, jobject thiz, jboolean available)
{

}

// Invoked when the RewardedVideo ad view has opened
JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdStartedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoStarted);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

// Invoked when the RewardedVideo ad view is about to be closed
JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdEndedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoEnded);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

// Invoked when the user completed the video and should be rewarded
JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdRewardedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::ReceivedReward);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

// Invoked when RewardedVideo call to show a rewarded video has failed
JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdShowFailedThunkCpp(JNIEnv* jenv, jobject thiz, jint errorCode, jstring errorMessage)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoShowFailed);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

JNI_METHOD void Java_com_ironsource_mediationsdk_sdk_RewardedVideoListener_onRewardedVideoAdClickedThunkCpp(JNIEnv* jenv, jobject thiz)
{
    if (ISProxy != nullptr)
    {
        ISProxy->VideoStateDelegate.Broadcast(EIronSourceEventType::VideoTapped);
    }
    else
    {
        LOGD("%s: invalid ISProxy", TCHAR_TO_ANSI(*VA_FUNC_LINE));
    }
}

#endif // WITH_IRONSOURCE && PLATFORM_ANDROID
