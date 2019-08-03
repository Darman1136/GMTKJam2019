// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Playback/PlaybackObject.h"
#include "PlaybackCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GMTK2019_API APlaybackCharacter : public APaperCharacter
{
	GENERATED_BODY()

		APlaybackCharacter();

		virtual void Tick(float DeltaSeconds) override;
public:
	void SetPlaybackTransform(TArray<FPlaybackTransformStruct> NewPlayerPlaybackTransform);

	void TogglePlayback();

	void StartPlayback();

	void UpdatePlayback();

	void UpdatePlaybackLerp();

private:
	TArray<FPlaybackTransformStruct> PlayerPlaybackTransform;

	float CurrentPlaybackMillis = 0;

	int32 PlaybackIndex = 0;

	bool bIsPlaybackRunning = false;

	FTransform CurrentPlaybackGoal;
	FTransform CurrentPlaybackStart;
	float CurrentPlaybackStartRelativeTimeToNextUpdate;
	float CurrentPlaybackGoalRelativeTimeToNextUpdate;
};
