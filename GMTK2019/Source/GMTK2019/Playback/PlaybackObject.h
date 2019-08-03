// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlaybackObject.generated.h"

USTRUCT()
struct FPlaybackTransformStruct {
	GENERATED_BODY()

	UPROPERTY()
		float RelativeMillisSincePlaybackStart;

	UPROPERTY()
		FTransform ActorTransform;

	void SetRelativeMillisSincePlaybackStart(float NewRelativeMillisSincePlaybackStart) {
		RelativeMillisSincePlaybackStart = NewRelativeMillisSincePlaybackStart;
	}

	void SetActorTransform(FTransform NewActorTransform) {
		ActorTransform = NewActorTransform;
	}

	FPlaybackTransformStruct() {
		RelativeMillisSincePlaybackStart = 0;
		ActorTransform = FTransform();
	}
};

/**
 * 
 */
UCLASS()
class GMTK2019_API UPlaybackObject : public UObject
{
	GENERATED_BODY()
	
};
