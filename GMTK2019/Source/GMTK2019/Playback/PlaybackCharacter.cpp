// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaybackCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(PlaybackCharacterLog, Log, All);

APlaybackCharacter::APlaybackCharacter() {
	StepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepAudioComponent"));
	StepAudioComponent->SetupAttachment(RootComponent);
	StepAudioComponent->bAutoActivate = false;
}

void APlaybackCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	CurrentPlaybackMillis += DeltaSeconds;

	if (PlayerPlaybackTransform.Num() > 0) {
		UpdatePlayback();
	}
	UpdateAnimation();
}

void APlaybackCharacter::UpdateAnimation() {
	if (bIsMovingRight) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	} else {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}

	EAnimationState DesiredAnimationState = bIsMoving ? EAnimationState::Anim_Run : EAnimationState::Anim_Stand;
	if (CurrentAnimationState != DesiredAnimationState) {
		UpdateAnimationToState(DesiredAnimationState);
	}
}

void APlaybackCharacter::UpdateAnimationToState(EAnimationState DesiredAnimationState) {
	CurrentAnimationState = DesiredAnimationState;
	UPaperFlipbook* DesiredAnimation = AnimationFlipbookMap.FindRef(DesiredAnimationState);
	if (DesiredAnimation) {
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

	if (CurrentAnimationState == EAnimationState::Anim_Run && !GetMovementComponent()->IsFalling()) {
		if (!StepAudioComponent->IsPlaying()) {
			StepAudioComponent->Play();
		}
	} else {
		StepAudioComponent->Stop();
	}
}

void APlaybackCharacter::TogglePlayback() {
	if (!bIsPlaybackRunning) {
		StartPlayback();
	} else {
		bIsPlaybackRunning = false;
	}
}

void APlaybackCharacter::StartPlayback() {
	UE_LOG(PlaybackCharacterLog, Warning, TEXT("startplayback"));
	PlaybackIndex = 0;
	CurrentPlaybackMillis = 0;
	bIsPlaybackRunning = true;
}

void APlaybackCharacter::SetPlaybackTransform(TArray<FPlaybackTransformStruct> NewPlayerPlaybackTransform) {
	PlayerPlaybackTransform = NewPlayerPlaybackTransform;
}

void APlaybackCharacter::UpdatePlayback() {
	if (!bIsPlaybackRunning) {
		return;
	}

	if (PlayerPlaybackTransform.Num() > PlaybackIndex) {
		FPlaybackTransformStruct PlaybackTransformStruct = PlayerPlaybackTransform[PlaybackIndex];
		if (PlaybackTransformStruct.RelativeMillisSincePlaybackStart < CurrentPlaybackMillis) {
			if (PlayerPlaybackTransform.Num() > PlaybackIndex + 1) {
				CurrentPlaybackStartRelativeTimeToNextUpdate = PlaybackTransformStruct.RelativeMillisSincePlaybackStart;
				CurrentPlaybackGoalRelativeTimeToNextUpdate = PlayerPlaybackTransform[PlaybackIndex + 1].RelativeMillisSincePlaybackStart;
				CurrentPlaybackStart = GetActorTransform();
				CurrentPlaybackGoal = PlaybackTransformStruct.ActorTransform;
			} else {
				SetActorTransform(PlaybackTransformStruct.ActorTransform);
			}
			PlaybackIndex++;
		}
		if (PlaybackIndex > 0) {
			UpdatePlaybackLerp();
		}
	} else {
		UE_LOG(PlaybackCharacterLog, Warning, TEXT("playback done"));
		bIsPlaybackRunning = false;
		bIsMoving = false;
	}
}

void APlaybackCharacter::UpdatePlaybackLerp() {
	FTransform NextTransform = CurrentPlaybackStart;

	float LerpStart = CurrentPlaybackMillis - CurrentPlaybackStartRelativeTimeToNextUpdate;
	float LerpEnd = CurrentPlaybackGoalRelativeTimeToNextUpdate - CurrentPlaybackStartRelativeTimeToNextUpdate;

	FVector NewLocation = FMath::Lerp(CurrentPlaybackStart.GetLocation(), CurrentPlaybackGoal.GetLocation(), LerpStart / LerpEnd);
	NextTransform.SetLocation(NewLocation);
	if (NextTransform.IsValid()) {
		FVector MovementVector = GetActorTransform().GetLocation() - NewLocation;
		bIsMoving = MovementVector.SizeSquared() > 0.1f;
		if (FMath::Abs(MovementVector.X) > 0.1f) {
			bIsMovingRight = MovementVector.X < 0.f;
		}
		SetActorTransform(NextTransform);
	}
}
