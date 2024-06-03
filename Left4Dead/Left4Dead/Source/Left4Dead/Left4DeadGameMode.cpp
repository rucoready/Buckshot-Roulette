// Copyright Epic Games, Inc. All Rights Reserved.

#include "Left4DeadGameMode.h"
#include "Left4DeadCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALeft4DeadGameMode::ALeft4DeadGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
