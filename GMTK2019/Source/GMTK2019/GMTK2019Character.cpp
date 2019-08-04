// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GMTK2019Character.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbook.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "LevelInfoActor.h"
#include "Runtime/Engine/Classes/Components/PostProcessComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGMTK2019Character

AGMTK2019Character::AGMTK2019Character() {
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TheMusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);
	MusicAudioComponent->bAutoActivate = false;

	StepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepAudioComponent"));
	StepAudioComponent->SetupAttachment(RootComponent);
	StepAudioComponent->bAutoActivate = false;

	WhooshAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WhooshAudioComponent"));
	WhooshAudioComponent->SetupAttachment(RootComponent);
	WhooshAudioComponent->bAutoActivate = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);


	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
	// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	// 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UClass> PbCharBlueprint(TEXT("Class'/Game/2DSideScrollerCPP/Blueprints/Playback/PlaybackCharacterBP.PlaybackCharacterBP_C'"));
	if (PbCharBlueprint.Object) {
		PlaybackCharacterBP = PbCharBlueprint.Object;
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AGMTK2019Character::UpdateAnimation() {
	if (!GetSprite()->GetFlipbook()) {
		return;
	}

	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	EAnimationState DesiredAnimationState = (PlayerSpeedSqr > 0.0f) ? EAnimationState::Anim_Run : EAnimationState::Anim_Stand;
	if (CurrentAnimationState != DesiredAnimationState) {
		UpdateAnimationToState(DesiredAnimationState);
	}

	int32 CurrentAnimationIndex = GetSprite()->GetFlipbook()->GetKeyFrameIndexAtTime(GetSprite()->GetPlaybackPosition());
	int32 LastFlipbookFrameIndex = GetSprite()->GetFlipbook()->GetNumKeyFrames() - 1;
	bool bIsAnimationDone = CurrentAnimationIndex == LastFlipbookFrameIndex;
	if (bIsAnimationDone && !bIsAnimationDoneToggle) {
		AnimationDone();
	} else if (bIsAnimationDone != bIsAnimationDoneToggle) {
		bIsAnimationDoneToggle = bIsAnimationDone;
	}
}

void AGMTK2019Character::UpdateAnimationToState(EAnimationState DesiredAnimationState) {
	CurrentAnimationState = DesiredAnimationState;
	UPaperFlipbook* DesiredAnimation = AnimationFlipbookMap.FindRef(DesiredAnimationState);
	if (DesiredAnimation) {
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

	if (CurrentAnimationState == EAnimationState::Anim_Run && !GetMovementComponent()->IsFalling()) {
		if (!StepAudioComponent->IsPlaying()) {
			StepAudioComponent->Play();
		}
	} else {
		StepAudioComponent->Stop();
	}
}

void AGMTK2019Character::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> LevelInfoActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelInfoActor::StaticClass(), LevelInfoActors);
	LevelInfo = Cast<ALevelInfoActor>(LevelInfoActors[0]);

	TheGameInstance = Cast<UGMTK2019GameInstance>(GetGameInstance());

	MusicAudioComponent->FadeIn(MusicFadeDelay, 1.f, TheGameInstance->GetMusicOffset());

	if (!LevelInfo->ClearPlaybacks()) {
		TheGameInstance->SpawnPlaybacks();
	} else {
		TheGameInstance->ClearPlaybackMap();
	}
	ToggleRecord();
}

void AGMTK2019Character::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	MillisSinceLevelStart += DeltaSeconds;
	TheGameInstance->AddMusicOffset(DeltaSeconds);

	UpdateCharacter();
}

void AGMTK2019Character::AnimationDone() {
	bIsAnimationDoneToggle = true;
	if (AnimationDelegate_OnFinish.IsBound()) {
		AnimationDelegate_OnFinish.Broadcast(CurrentAnimationState);
	}
}

void AGMTK2019Character::StartRecordingPlayer() {
	MillisSinceLevelStart = 0.f;
	PlayerRecordingTransform.Empty();
	GetWorld()->GetTimerManager().SetTimer(RecordPlayerTimerHandle, this, &AGMTK2019Character::RecordPlayer, RecordPlayerDelay / 1000.f, true);
}

void AGMTK2019Character::RecordPlayer() {
	UE_LOG(SideScrollerCharacter, Warning, TEXT("%f ms"), MillisSinceLevelStart);
	FPlaybackTransformStruct PlaybackObject;
	PlaybackObject.SetRelativeMillisSincePlaybackStart(MillisSinceLevelStart);
	PlaybackObject.SetActorTransform(GetActorTransform());
	PlayerRecordingTransform.Add(PlaybackObject);
}

void AGMTK2019Character::ToggleRecord() {
	bIsRecording = !bIsRecording;
	if (bIsRecording) {
		StartRecordingPlayer();
	} else {
		GetWorld()->GetTimerManager().ClearTimer(RecordPlayerTimerHandle);
		RecordPlayer();
		if (TheGameInstance) {
			TheGameInstance->AddToPlaybackMap(LevelInfo->GetCurrentLevelIndex(), PlayerRecordingTransform);
		}
	}
}

void AGMTK2019Character::SpawnPlaybackCharacter() {
	TArray<FPlaybackTransformStruct> PlayerPlaybackTransform = TheGameInstance->GetEntryInPlaybackMap(0);
	if (PlayerPlaybackTransform.Num() > 0) {
		FActorSpawnParameters SpawnInfo;
		FVector StartLocation = PlayerPlaybackTransform[0].ActorTransform.GetLocation();
		APlaybackCharacter* SpawnedActor = GetWorld()->SpawnActor<APlaybackCharacter>(PlaybackCharacterBP, StartLocation, FRotator(), SpawnInfo);
		if (SpawnedActor) {
			SpawnedActor->SetPlaybackTransform(PlayerPlaybackTransform);
			SpawnedActor->TogglePlayback();
		}
	}
}


void AGMTK2019Character::Use() {
	if (GetMovementComponent()->IsFalling()) {
		UE_LOG(SideScrollerCharacter, Log, TEXT("Cant 'use' while in the air"));
		return;
	}
	if (PlayerPressUseDelegate_OnPress.IsBound()) {
		PlayerPressUseDelegate_OnPress.Broadcast();
	}
}

void AGMTK2019Character::ResetLevel() {
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UE_LOG(SideScrollerCharacter, Log, TEXT("Reset %s"), *LevelName);
	ChangeLevel(FName(*LevelName));

}

void AGMTK2019Character::PreviousLevel() {
	if (PlayerPressPreviousLevelDelegate_OnPress.IsBound()) {
		PlayerPressPreviousLevelDelegate_OnPress.Broadcast();
	}

	if (LevelInfo->GetPreviousLevelName() != NAME_None) {
		TheGameInstance->RemoveEntryFromPlaybackMap(LevelInfo->GetCurrentLevelIndex() - 1);
		ChangeLevel(LevelInfo->GetPreviousLevelName());
	}
}

void AGMTK2019Character::ChangeLevel(FName LevelName) {
	WhooshAudioComponent->Play();
	float WantedOffset = TheGameInstance->GetMusicOffset();

	if (WantedOffset > MusicSoundCue->Duration - 10.f) {
		TheGameInstance->ResetMusicOffset();
	}

	MusicAudioComponent->FadeOut(MusicFadeDelay, 0.f);

	FTimerDelegate TimerDelegate;
	FTimerHandle DelayHandle;
	TimerDelegate.BindUFunction(this, FName("ActualChangeLevel"), LevelName);
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, TimerDelegate, MusicFadeDelay, false);
}

void AGMTK2019Character::ActualChangeLevel(FName LevelName) {
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGMTK2019Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGMTK2019Character::MoveRight);
	/*
	PlayerInputComponent->BindAction("TogglePlayback", IE_Pressed, this, &AGMTK2019Character::SpawnPlaybackCharacter);
	PlayerInputComponent->BindAction("ToggleRecord", IE_Pressed, this, &AGMTK2019Character::ToggleRecord);
	*/
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AGMTK2019Character::Use);
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &AGMTK2019Character::ResetLevel);
	PlayerInputComponent->BindAction("PreviousLevel", IE_Pressed, this, &AGMTK2019Character::PreviousLevel);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &AGMTK2019Character::ToggleMenu);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGMTK2019Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGMTK2019Character::TouchStopped);
}

void AGMTK2019Character::MoveRight(float Value) {
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AGMTK2019Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location) {
	// Jump on any touch
	Jump();
}

void AGMTK2019Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location) {
	// Cease jumping once touch stopped
	StopJumping();
}

void AGMTK2019Character::UpdateCharacter() {
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr) {
		if (TravelDirection < 0.0f) {
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		} else if (TravelDirection > 0.0f) {
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AGMTK2019Character::SetScreenBlack() {
	SideViewCameraComponent->PostProcessSettings.bOverride_ColorGamma = true;
	SideViewCameraComponent->PostProcessSettings.ColorGamma = FVector4(0.f, 0.f, 0.f, 0.f);
}

void AGMTK2019Character::ToggleMenu() {
	bIsMenuVisible = !bIsMenuVisible;
	if (EscapedPressedDelegate_OnPress.IsBound()) {
		EscapedPressedDelegate_OnPress.Broadcast(bIsMenuVisible);
	}
}
