// Copyright 2015-2022 MY.GAMES. All Rights Reserved.

#include "PsIronSourceLibrary.h"

#include "IPsIronSource.h"

UPsIronSourceLibrary::UPsIronSourceLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPsIronSourceProxy* UPsIronSourceLibrary::GetIronSourceProxy()
{
	return IPsIronSource::Get().IronSourceProxy;
}
