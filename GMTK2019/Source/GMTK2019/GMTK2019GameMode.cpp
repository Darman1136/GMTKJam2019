// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GMTK2019GameMode.h"
#include "GMTK2019Character.h"
#include "HUD/GMTK2019HUD.h"

AGMTK2019GameMode::AGMTK2019GameMode() {
	// Set default pawn class to our character
	DefaultPawnClass = AGMTK2019Character::StaticClass();

	static ConstructorHelpers::FObjectFinder<UClass> HUDBP(TEXT("Class'/Game/2DSideScrollerCPP/Blueprints/HUD/GMTK2019HUDBP.GMTK2019HUDBP_C'"));
	if (HUDBP.Object) {
		HUDClass = HUDBP.Object;
	} else {
		HUDClass = AGMTK2019HUD::StaticClass();
	}
}
