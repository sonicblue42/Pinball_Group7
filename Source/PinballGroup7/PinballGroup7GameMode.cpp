// Copyright Epic Games, Inc. All Rights Reserved.

#include "PinballGroup7GameMode.h"
#include "PinballGroup7PlayerController.h"
#include "PinballGroup7Pawn.h"

APinballGroup7GameMode::APinballGroup7GameMode()
{
	// no pawn by default
	DefaultPawnClass = APinballGroup7Pawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = APinballGroup7PlayerController::StaticClass();
}
