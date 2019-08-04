// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelFinishActor.h"
#include "GMTK2019Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LevelFinishActorLog, Log, All);

ALevelFinishActor::ALevelFinishActor() : Super() {
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetRelativeTransform(FTransform());
	BoxComponent->SetupAttachment(RootComponent);
}

void ALevelFinishActor::BeginPlay() {
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelFinishActor::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALevelFinishActor::OnEndOverlap);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter && PlayerCharacter->IsA(AGMTK2019Character::StaticClass())) {
		Cast< AGMTK2019Character>(PlayerCharacter)->PlayerPressUseDelegate_OnPress.AddDynamic(this, &ALevelFinishActor::DoActionWhenPlayerOverlaps);
	}
}

void ALevelFinishActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AGMTK2019Character::StaticClass())) {
		bIsPlayerOverlapping = true;
	}
}

void ALevelFinishActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AGMTK2019Character::StaticClass())) {
		bIsPlayerOverlapping = false;
	}
}

void ALevelFinishActor::DoActionWhenPlayerOverlaps() {
	if (NextLevelName == NAME_None) {
		UE_LOG(LevelFinishActorLog, Log, TEXT("NEXT LEVEL NAME NOT SET"))
			return;
	}
	if (bIsPlayerOverlapping) {
		UE_LOG(LevelFinishActorLog, Log, TEXT("DoActionWhenPlayerOverlaps"));
		UGameplayStatics::OpenLevel(GetWorld(), "Level2");
	}
}
