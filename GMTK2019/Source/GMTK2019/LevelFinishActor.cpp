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
		AGMTK2019Character* TheCharacter = Cast< AGMTK2019Character>(PlayerCharacter);
		TheCharacter->PlayerPressUseDelegate_OnPress.AddDynamic(this, &ALevelFinishActor::DoActionWhenPlayerOverlaps);
		TheCharacter->PlayerPressPreviousLevelDelegate_OnPress.AddDynamic(this, &ALevelFinishActor::LoadPreviousLevel);
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
	if (bIsPlayerOverlapping) {
		Load(NextLevelName);
	}
}

void ALevelFinishActor::LoadPreviousLevel() {
	Load(PreviousLevelName);
}

void ALevelFinishActor::Load(FName LevelName) {
	if (LevelName == NAME_None) {
		UE_LOG(LevelFinishActorLog, Log, TEXT("LEVEL NAME NOT SET"));
		return;
	}
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter && PlayerCharacter->IsA(AGMTK2019Character::StaticClass())) {
		Cast<AGMTK2019Character>(PlayerCharacter)->ToggleRecord();
	}
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
