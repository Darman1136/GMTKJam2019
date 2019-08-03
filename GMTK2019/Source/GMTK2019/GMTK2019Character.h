// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Playback/PlaybackObject.h"
#include "GMTK2019Character.generated.h"

class UTextRenderComponent;



UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAnimationState : uint8 {
	Anim_Stand,
	Anim_Run
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimationFinishDelegate, EAnimationState, AnimationState);

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

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(BlueprintAssignable, Category = "Animation")
		FAnimationFinishDelegate AnimationDelegate_OnFinish;


private:
	void AnimationDone();

	void UpdateAnimationToState(EAnimationState DesiredAnimationState);

	void StartRecordingPlayer();

	void RecordPlayer();

	void ToggleRecord();

	void TogglePlayback();

	void StartPlayback();

	void UpdatePlayback();

	void UpdatePlaybackLerp();

private:
	EAnimationState CurrentAnimationState = EAnimationState::Anim_Stand;

	bool bIsAnimationDoneToggle = false;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TMap<EAnimationState, class UPaperFlipbook*> AnimationFlipbookMap;

	TArray<FPlaybackTransformStruct> PlayerRecordingTransform;

	float MillisSinceLevelStart = 0;

	FTimerHandle RecordPlayerTimerHandle;

	UPROPERTY(EditAnywhere)
		float RecordPlayerDelay = 100;

	float CurrentPlaybackMillis = 0;

	bool bIsPlaybackRunning = false;

	bool bIsRecording = false;

	int32 PlaybackIndex = 0;

	FTransform CurrentPlaybackGoal;
	FTransform CurrentPlaybackStart;
	float CurrentPlaybackStartRelativeTimeToNextUpdate;
	float CurrentPlaybackGoalRelativeTimeToNextUpdate;
};
