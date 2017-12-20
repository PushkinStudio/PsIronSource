// Copyright 2017 Pushkin Studio. All Rights Reserved.

#include "IronSourcePrivatePCH.h"
#include "IronSourceLibrary.h"

UIronSourceLibrary::UIronSourceLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UIronSourceProxy* UIronSourceLibrary::GetIronSourceProxy()
{
	return IIronSource::Get().IronSourceProxy;
}
