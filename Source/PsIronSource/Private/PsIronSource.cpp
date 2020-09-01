// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

#include "IPsIronSource.h"

#include "PsIronSourceDefines.h"
#include "PsIronSourceProxy.h"
#include "PsIronSourceSettings.h"
#if WITH_IRONSOURCE
#if PLATFORM_IOS
#include "PsIronSource_iOS.h"
#elif PLATFORM_ANDROID
// ZEN-5906
//#include "PsIronSource_Android.h"
#endif // PLATFORM_ANDROID
#endif // WITH_IRONSOURCE

#include "ISettingsModule.h"
#include "Misc/ConfigCacheIni.h"
#include "UObject/Package.h"

#define LOCTEXT_NAMESPACE "IronSource"

class FPsIronSource : public IPsIronSource
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		ISSettings = NewObject<UPsIronSourceSettings>(GetTransientPackage(), "IronSourceSettings", RF_Standalone);
		ISSettings->AddToRoot();

		// Register settings
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "IronSource",
				LOCTEXT("RuntimeSettingsName", "IronSource"),
				LOCTEXT("RuntimeSettingsDescription", "Configure API keys for IronSource"),
				ISSettings);
		}

		// Proxy class depends on platform
		UClass* PlatformClass = UPsIronSourceProxy::StaticClass();
#if WITH_IRONSOURCE
#if PLATFORM_IOS
		PlatformClass = UPsIronSource_iOS::StaticClass();
#elif PLATFORM_ANDROID
		// ZEN-5906
		//PlatformClass = UPsIronSource_Android::StaticClass();
#endif // PLATFORM_ANDROID
#endif // WITH_IRONSOURCE

		// Create IronSource proxy and initalize module by default
		IronSourceProxy = NewObject<UPsIronSourceProxy>(GetTransientPackage(), PlatformClass);
		IronSourceProxy->SetFlags(RF_Standalone);
		IronSourceProxy->AddToRoot();
	}

	virtual void ShutdownModule() override
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "IronSource");
		}

		if (!GExitPurge)
		{
			// If we're in exit purge, this object has already been destroyed
			IronSourceProxy->RemoveFromRoot();
			ISSettings->RemoveFromRoot();
		}
		else
		{
			IronSourceProxy = nullptr;
			ISSettings = nullptr;
		}
	}

private:
	/** Settings */
	UPsIronSourceSettings* ISSettings;
};

IMPLEMENT_MODULE(FPsIronSource, PsIronSource);

DEFINE_LOG_CATEGORY(LogPsIronSource);

#undef LOCTEXT_NAMESPACE
