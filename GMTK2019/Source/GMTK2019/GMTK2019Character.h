// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Playback/PlaybackObject.h"
#include "Playback/PlaybackCharacter.h"
#include "GMTK2019GameInstance.h"
#include "LevelInfoActor.h"
#include "GMTK2019Enums.h"
#include "Audio/MusicAudioComponent.h"
#include "GMTK2019Character.generated.h"

class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationFinishDelegate, EAnimationState, AnimationState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerPressUse);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerPressPreviousLevel);

/**
 * This class is the default character for GMTK2019, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config = Game)
class AGMTK2019Character : public APaperCharacter {
	GENERATED_BODY()

		/** Side view camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

public:
	AGMTK2019Character();

	void ToggleRecord();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE class UAudioComponent* GetMusicAudioComponent() const { return MusicAudioComponent; }

	void ResetLevel();

	UFUNCTION(BlueprintCallable)
		void SetScreenBlack();

	void ChangeLevel(FName LevelName);

	UFUNCTION()
		void ActualChangeLevel(FName LevelName);

public:
	UPROPERTY(BlueprintAssignable, Category = "Animation")
		FAnimationFinishDelegate AnimationDelegate_OnFinish;

	UPROPERTY(BlueprintAssignable, Category = "Player|Input")
		FPlayerPressUse PlayerPressUseDelegate_OnPress;

	UPROPERTY(BlueprintAssignable, Category = "Player|Input")
		FPlayerPressPreviousLevel PlayerPressPreviousLevelDelegate_OnPress;

private:
	void AnimationDone();

	void UpdateAnimationToState(EAnimationState DesiredAnimationState);

	void StartRecordingPlayer();

	void RecordPlayer();

	void SpawnPlaybackCharacter();

	void Use();

	void PreviousLevel();

private:
	UGMTK2019GameInstance* TheGameInstance;

	EAnimationState CurrentAnimationState = EAnimationState::Anim_Stand;

	bool bIsAnimationDoneToggle = false;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TMap<EAnimationState, class UPaperFlipbook*> AnimationFlipbookMap;

	TArray<FPlaybackTransformStruct> PlayerRecordingTransform;

	float MillisSinceLevelStart = 0;

	FTimerHandle RecordPlayerTimerHandle;

	UPROPERTY(EditAnywhere)
		float RecordPlayerDelay = 100;

	bool bIsRecording = false;

	TSubclassOf<class APlaybackCharacter> PlaybackCharacterBP;

	ALevelInfoActor* LevelInfo;

	UPROPERTY(EditAnywhere)
		UAudioComponent* MusicAudioComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* StepAudioComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* WhooshAudioComponent;

	UPROPERTY(EditAnywhere)
		float MusicFadeDelay = .1f;

	UPROPERTY(EditAnywhere)
		USoundBase* MusicSoundCue;
};
