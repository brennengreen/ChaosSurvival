// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChaosSurvivalGameMode.h"
#include "ChaosSurvivalPlayerController.h"
#include "ChaosSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChaosSurvivalGameMode::AChaosSurvivalGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AChaosSurvivalPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}