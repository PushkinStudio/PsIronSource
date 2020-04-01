// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "PsIronSourceLibrary.h"

UPsIronSourceLibrary::UPsIronSourceLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPsIronSourceProxy* UPsIronSourceLibrary::GetIronSourceProxy()
{
	return IPsIronSource::Get().IronSourceProxy;
}
