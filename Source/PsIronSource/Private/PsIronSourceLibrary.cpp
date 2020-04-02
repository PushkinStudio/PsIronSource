// Copyright 2015-2020 Mail.Ru Group. All Rights Reserved.

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
