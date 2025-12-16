// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

APangaeaGameMode::APangaeaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}