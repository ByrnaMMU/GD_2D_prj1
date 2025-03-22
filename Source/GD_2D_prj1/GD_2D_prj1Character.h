// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "IdleState.h"
#include "WalkState.h"
#include "JumpState.h"
#include "FallState.h"
#include "PaperFlipbook.h"
#include "GD_2D_prj1Character.generated.h"



class UTextRenderComponent;
class UInputMappingContext;
class UInputAction;


////enums for character
//UENUM(BlueprintType)
//enum class ECharacterState : uint8
//{
//	Idle		UMETA(DisplayName = "Idle"),
//	Running		UMETA(DisplayName = "Running"),
//	Jumping		UMETA(DisplayName = "Jumping"),
//	Falling		UMETA(DisplayName = "Falling"),
//	Dead		UMETA(DisplayName = "Dead")
//};

/**
 * This class is the default character for GD_2D_prj1, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AGD_2D_prj1Character : public APaperCharacter
{
	GENERATED_BODY()


	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;
protected:
	


	/** Called for side to side input */
	void MoveRight(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	//void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	//State machine methods
	//void UpdateState();
	//void HandleState();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	

	


public:
	AGD_2D_prj1Character();


	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation(UPaperFlipbook* animation);

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpingAnimation;

	// The animation to play while falling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* FallingAnimation;

	//character states
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	//ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere)
	// code for controlling the stamina of the player
	int Stamina;
	UPROPERTY(EditAnywhere)
	int MaxStamina;
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// The Players input mapping context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UCharacterState* CurrentState;

	// the idle state of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UCharacterState* IdleState;
	// the walk state of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UCharacterState* WalkState;
	// the jump state of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UCharacterState* JumpState;
	// the fall state of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UCharacterState* FallState;

	// State management
	void SetState(UCharacterState* NewState);
	UCharacterState* GetCurrentState() const;
};
