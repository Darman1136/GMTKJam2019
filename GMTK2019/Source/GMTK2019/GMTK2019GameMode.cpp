// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GMTK2019GameMode.h"
#include "GMTK2019Character.h"

AGMTK2019GameMode::AGMTK2019GameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGMTK2019Character::StaticClass();	
}
