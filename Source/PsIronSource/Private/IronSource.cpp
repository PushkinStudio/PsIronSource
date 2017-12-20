// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"

#include "UObject/Package.h"
#include "Misc/ConfigCacheIni.h"

#include "IronSourceProxy.h"
#include "IronSourceSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "IronSource"

class FIronSource : public IIronSource
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		ISSettings = NewObject<UIronSourceSettings>(GetTransientPackage(), "IronSourceSettings", RF_Standalone);
		ISSettings->AddToRoot();

		// Register settings
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "IronSource",
				LOCTEXT("RuntimeSettingsName", "IronSource"),
				LOCTEXT("RuntimeSettingsDescription", "Configure API keys for IronSource"),
				ISSettings
			);
		}

		// Proxy class depends on platform
		UClass* PlatformClass = UIronSourceProxy::StaticClass();
#if WITH_IRONSOURCE
#if PLATFORM_IOS
		PlatformClass = UIronSource_iOS::StaticClass();
#elif PLATFORM_ANDROID
		PlatformClass = UIronSource_Android::StaticClass();
#endif // PLATFORM_ANDROID
#endif // WITH_IRONSOURCE

		// Create IronSource proxy and initalize module by default
		IronSourceProxy = NewObject<UIronSourceProxy>(GetTransientPackage(), PlatformClass);
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
	UIronSourceSettings* ISSettings;

}; 

IMPLEMENT_MODULE(FIronSource, PsIronSource);

DEFINE_LOG_CATEGORY(LogIronSource);

#undef LOCTEXT_NAMESPACE
