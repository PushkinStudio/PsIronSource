// Copyright 2017 Pushkin Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPsIronSource, Log, All);

#define PSIS_FUNC (FString(__FUNCTION__))                  // Current Class Name + Function Name where this is called
#define PSIS_LINE (FString::FromInt(__LINE__))             // Current Line Number in the code where this is called
#define PSIS_FUNC_LINE (PSIS_FUNC + "(" + PSIS_LINE + ")") // Current Class and Line Number where this is called!
