// Copyright Epic Games, Inc. All Rights Reserved.

#include "fps_2GameMode.h"
#include "fps_2Character.h"
#include "UObject/ConstructorHelpers.h"

Afps_2GameMode::Afps_2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
