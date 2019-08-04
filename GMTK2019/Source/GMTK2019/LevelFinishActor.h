// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "LevelFinishActor.generated.h"

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

	UFUNCTION()
		void LoadPreviousLevel();

private:
	void Load(FName LevelName);

private:
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	bool bIsPlayerOverlapping;

	UPROPERTY(EditAnywhere, Category = "Levelname")
		FName NextLevelName;

	UPROPERTY(EditAnywhere, Category = "Levelname")
		FName PreviousLevelName;
};
