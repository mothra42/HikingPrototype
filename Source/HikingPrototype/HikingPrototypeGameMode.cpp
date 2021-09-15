// Copyright Epic Games, Inc. All Rights Reserved.

#include "HikingPrototypeGameMode.h"
#include "HikingPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHikingPrototypeGameMode::AHikingPrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
