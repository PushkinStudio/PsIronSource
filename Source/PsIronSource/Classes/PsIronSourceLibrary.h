// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

#pragma once

#include "PsIronSourceProxy.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "PsIronSourceLibrary.generated.h"

UCLASS()
class PSIRONSOURCE_API UPsIronSourceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** Get IronSource proxy object */
	UFUNCTION(BlueprintCallable, Category = IronSource, BlueprintPure)
	static UPsIronSourceProxy* GetIronSourceProxy();
};
