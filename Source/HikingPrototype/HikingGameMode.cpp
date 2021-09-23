// Copyright Epic Games, Inc. All Rights Reserved.

#include "HikingGameMode.h"
#include "Hiker.h"
#include "UObject/ConstructorHelpers.h"

AHikingGameMode::AHikingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/Hiker_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
