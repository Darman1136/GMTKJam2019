// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Playback/PlaybackObject.h"
#include "GMTK2019Enums.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
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
	void UpdateAnimation();

	void UpdateAnimationToState(EAnimationState DesiredAnimationState);

private:
	EAnimationState CurrentAnimationState = EAnimationState::Anim_Stand;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TMap<EAnimationState, class UPaperFlipbook*> AnimationFlipbookMap;

	TArray<FPlaybackTransformStruct> PlayerPlaybackTransform;

	float CurrentPlaybackMillis = 0;

	int32 PlaybackIndex = 0;

	bool bIsPlaybackRunning = false;

	FTransform CurrentPlaybackGoal;
	FTransform CurrentPlaybackStart;
	float CurrentPlaybackStartRelativeTimeToNextUpdate;
	float CurrentPlaybackGoalRelativeTimeToNextUpdate;

	bool bIsMoving = false;
	bool bIsMovingRight = true;

	UPROPERTY(EditAnywhere)
		UAudioComponent* StepAudioComponent;
};
