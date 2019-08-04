// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelResetBox.h"
#include "GMTK2019Character.h"
#include "Runtime/Engine/Classes/Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"

ALevelResetBox::ALevelResetBox() {
	SplashAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepAudioComponent"));
	SplashAudioComponent->SetupAttachment(RootComponent);
	SplashAudioComponent->bAutoActivate = false;
}

void ALevelResetBox::BeginPlay() {
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALevelResetBox::ActorOverlapped);
	SplashAudioComponent->OnAudioFinished.AddDynamic(this, &ALevelResetBox::Reset);
}

void ALevelResetBox::ActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AGMTK2019Character::StaticClass())) {
		Player = Cast<AGMTK2019Character>(OtherActor);
		Player->SetScreenBlack();
		if (SplashAudioComponent->Sound) {
			SplashAudioComponent->Play();
		} else {
			Reset();
		}
	}
}

void ALevelResetBox::Reset() {
	Player->ResetLevel();
}
