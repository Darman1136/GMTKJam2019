// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "GMTK2019Character.h"
#include "LevelResetBox.generated.h"

/**
 *
 */
UCLASS()
class GMTK2019_API ALevelResetBox : public ATriggerBox {
	GENERATED_BODY()

		ALevelResetBox();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void ActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Reset();

private:
	UPROPERTY(EditAnywhere)
		UAudioComponent* SplashAudioComponent;

	AGMTK2019Character* Player;
};
