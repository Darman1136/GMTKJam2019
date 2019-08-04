// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "MusicAudioComponent.generated.h"

/**
 * 
 */
UCLASS()
class GMTK2019_API UMusicAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

		UMusicAudioComponent();

	virtual void BeginPlay() override;
};
