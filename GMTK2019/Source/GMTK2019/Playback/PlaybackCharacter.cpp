// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaybackCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(PlaybackCharacterLog, Log, All);

APlaybackCharacter::APlaybackCharacter() {
	//
}

void APlaybackCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	CurrentPlaybackMillis += DeltaSeconds;

	// UpdateCharacter();
	if (PlayerPlaybackTransform.Num() > 0) {
		UpdatePlayback();
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
	}
}

void APlaybackCharacter::UpdatePlaybackLerp() {
	FTransform NextTransform = CurrentPlaybackStart;

	float LerpStart = CurrentPlaybackMillis - CurrentPlaybackStartRelativeTimeToNextUpdate;
	float LerpEnd = CurrentPlaybackGoalRelativeTimeToNextUpdate - CurrentPlaybackStartRelativeTimeToNextUpdate;

	UE_LOG(PlaybackCharacterLog, Warning, TEXT("start %f"), LerpStart);
	UE_LOG(PlaybackCharacterLog, Warning, TEXT("end %f ms"), LerpEnd);

	FVector NewLocation = FMath::Lerp(CurrentPlaybackStart.GetLocation(), CurrentPlaybackGoal.GetLocation(), LerpStart / LerpEnd);
	NextTransform.SetLocation(NewLocation);
	UE_LOG(PlaybackCharacterLog, Warning, TEXT("MyCharacter's Location is %s"), *NextTransform.GetLocation().ToString());
	if (NextTransform.IsValid()) {
		SetActorTransform(NextTransform);
	}
}
