// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GMTK2019Enums.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAnimationState : uint8 {
	Anim_Stand,
	Anim_Run
};

/**
 * 
 */
UCLASS()
class GMTK2019_API UGMTK2019Enums : public UObject
{
	GENERATED_BODY()
	
};
