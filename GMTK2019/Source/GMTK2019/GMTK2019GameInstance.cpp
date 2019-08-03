// Fill out your copyright notice in the Description page of Project Settings.


#include "GMTK2019GameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Playback/PlaybackCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(GMTK2019GameInstanceLog, Log, All);

UGMTK2019GameInstance::UGMTK2019GameInstance() {
	static ConstructorHelpers::FObjectFinder<UClass> PbCharBlueprint(TEXT("Class'/Game/2DSideScrollerCPP/Blueprints/Playback/PlaybackCharacterBP.PlaybackCharacterBP_C'"));
	if (PbCharBlueprint.Object) {
		PlaybackCharacterBP = PbCharBlueprint.Object;
	}
}

void UGMTK2019GameInstance::AddToPlaybackMap(int32 index, TArray<FPlaybackTransformStruct> NewPlaybackTransformStructArray) {
	PlaybackMap.Add(index, NewPlaybackTransformStructArray);
	UE_LOG(GMTK2019GameInstanceLog, Log, TEXT("Added entry with id %d"), index);
}

void UGMTK2019GameInstance::AddToPlaybackMap(TArray<FPlaybackTransformStruct> NewPlaybackTransformStructArray) {
	UE_LOG(GMTK2019GameInstanceLog, Log, TEXT("Added entry with id %d"), PlaybackMap.Num());
	PlaybackMap.Add(PlaybackMap.Num(), NewPlaybackTransformStructArray);
}

TArray<FPlaybackTransformStruct> UGMTK2019GameInstance::GetEntryInPlaybackMap(int32 index) {
	if (PlaybackMap.Contains(index)) {
		UE_LOG(GMTK2019GameInstanceLog, Log, TEXT("Entry with id %d returned"), index);
		return PlaybackMap.FindRef(index);
	}
	UE_LOG(GMTK2019GameInstanceLog, Error, TEXT("Entry with id %d NOT returned, index not found"), index);
	TArray<FPlaybackTransformStruct> Empty;
	return Empty;
}

void UGMTK2019GameInstance::RemoveEntryFromPlaybackMap(int32 index) {
	if (PlaybackMap.Contains(index)) {
		PlaybackMap.Remove(index);
		UE_LOG(GMTK2019GameInstanceLog, Log, TEXT("Entry with id %d removed"), index);
	} else {
		UE_LOG(GMTK2019GameInstanceLog, Error, TEXT("Entry with id %d NOT removed, index not found"), index);
	}
}

void UGMTK2019GameInstance::ClearPlaybackMap() {
	PlaybackMap.Empty();
}

void UGMTK2019GameInstance::SpawnPlaybacks() {
	for (const TPair<int32, TArray<FPlaybackTransformStruct>>& Pair : PlaybackMap) {
		Spawn(Pair.Value);
	}
}

void UGMTK2019GameInstance::MusicStarted() {
	bMusicStarted = true;
}

bool UGMTK2019GameInstance::HasMusicStarted() {
	return bMusicStarted;
}

void UGMTK2019GameInstance::Spawn(TArray<FPlaybackTransformStruct> PlayerPlaybackTransform) {
	if (PlayerPlaybackTransform.Num() > 0) {
		FActorSpawnParameters SpawnInfo;
		FVector StartLocation = PlayerPlaybackTransform[0].ActorTransform.GetLocation();
		APlaybackCharacter* SpawnedActor = GetWorld()->SpawnActor<APlaybackCharacter>(PlaybackCharacterBP, StartLocation, FRotator(0.f, 0.f, 0.f), SpawnInfo);
		if (SpawnedActor) {
			SpawnedActor->SetPlaybackTransform(PlayerPlaybackTransform);
			SpawnedActor->TogglePlayback();
		}
	}
}
