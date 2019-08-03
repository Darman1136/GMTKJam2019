// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelInfoActor.generated.h"

UCLASS()
class GMTK2019_API ALevelInfoActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelInfoActor();

	FORCEINLINE class FName GetNextLevelName() const { return NextLevelName; }

	FORCEINLINE class FName GetPreviousLevelName() const { return PreviousLevelName; }

	FORCEINLINE int32 GetCurrentLevelIndex() const { return CurrentLevelIndex; }

	FORCEINLINE bool ClearPlaybacks() const { return bClearPlaybacks; }

private:
	UPROPERTY(EditAnywhere, Category = "Level")
		FName NextLevelName;

	UPROPERTY(EditAnywhere, Category = "Level")
		FName PreviousLevelName;

	UPROPERTY(EditAnywhere, Category = "Level")
		int32 CurrentLevelIndex;

	UPROPERTY(EditAnywhere, Category = "Level")
		bool bClearPlaybacks;
};
