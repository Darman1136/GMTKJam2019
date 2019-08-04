// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "LevelInfoActor.h"
#include "LevelFinishActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelReadyToFinish, bool, IsReady);

/**
 *
 */
UCLASS()
class GMTK2019_API ALevelFinishActor : public APaperFlipbookActor {
	GENERATED_BODY()

		ALevelFinishActor();

	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void DoActionWhenPlayerOverlaps();

public:
	UPROPERTY(BlueprintAssignable, Category = "Level")
		FLevelReadyToFinish LevelReadyToFinish_OnChange;

private:
	void Load(FName LevelName);

	UFUNCTION()
		void TriggerFinish();

private:
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	bool bIsPlayerOverlapping;

	ALevelInfoActor* LevelInfo;

	UPROPERTY(EditAnywhere)
		UAudioComponent* SuccessAudioComponent;
};
