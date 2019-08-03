// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Playback/PlaybackObject.h"
#include "GMTK2019GameInstance.generated.h"

/**
 *
 */
UCLASS()
class GMTK2019_API UGMTK2019GameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	void AddToPlaybackMap(int32 index, TArray<FPlaybackTransformStruct> NewPlaybackTransformStructArray);

	TArray<FPlaybackTransformStruct> GetEntryInPlaybackMap(int32 index);

	void RemoveEntryFromPlaybackMap(int32 index);

	void ClearPlaybackMap();

private:
		TMap<int32, TArray<FPlaybackTransformStruct>> PlaybackMap;
};
