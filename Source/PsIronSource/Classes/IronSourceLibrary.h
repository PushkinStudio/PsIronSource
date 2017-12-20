// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "IronSourceLibrary.generated.h"

UCLASS()
class PSIRONSOURCE_API UIronSourceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** Get IronSource proxy object */
	UFUNCTION(BlueprintCallable, Category = IronSource, BlueprintPure)
	static UIronSourceProxy* GetIronSourceProxy();
};
