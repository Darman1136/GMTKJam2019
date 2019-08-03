// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GMTK2019GameMode.h"
#include "GMTK2019Character.h"
#include "HUD/GMTK2019HUD.h"

AGMTK2019GameMode::AGMTK2019GameMode() {
	// Set default pawn class to our character
	DefaultPawnClass = AGMTK2019Character::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> HUDBP(TEXT("Blueprint'/Game/2DSideScrollerCPP/Blueprints/HUD/GMTK2019HUDBP.GMTK2019HUDBP'"));
	if (HUDBP.Object) {
		HUDClass = (UClass*)HUDBP.Object->GeneratedClass;
	} else {
		HUDClass = AGMTK2019HUD::StaticClass();
	}
}
