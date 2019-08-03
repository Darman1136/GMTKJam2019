// Fill out your copyright notice in the Description page of Project Settings.


#include "GMTK2019GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(GMTK2019GameInstanceLog, Log, All);

void UGMTK2019GameInstance::AddToPlaybackMap(int32 index, TArray<FPlaybackTransformStruct> NewPlaybackTransformStructArray) {
	PlaybackMap.Add(index, NewPlaybackTransformStructArray);
	UE_LOG(GMTK2019GameInstanceLog, Log, TEXT("Added entry with id %d"), index);
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
