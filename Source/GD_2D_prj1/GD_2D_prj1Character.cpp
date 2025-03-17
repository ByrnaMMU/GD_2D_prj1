// Copyright Epic Games, Inc. All Rights Reserved.

#include "GD_2D_prj1Character.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IdleState.h"
#include "PaperFlipbook.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGD_2D_prj1Character

AGD_2D_prj1Character::AGD_2D_prj1Character()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 65.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 0.5f;
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

	// create and attach the Idle state
	IdleState = CreateDefaultSubobject<UIdleState>(TEXT("IdleState"));
}

//////////////////////////////////////////////////////////////////////////
// Animation

void AGD_2D_prj1Character::UpdateAnimation(UPaperFlipbook*animation)
{
	// Are we playing this animation?
	if( GetSprite()->GetFlipbook() != animation)
	{
		GetSprite()->SetFlipbook(animation);
	}
}

void AGD_2D_prj1Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// update the players state
	CurrentState->UpdateState(DeltaSeconds);
	// update the state functionality
	CurrentState->HandleState();
	//UpdateCharacter();
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGD_2D_prj1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	/*PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGD_2D_prj1Character::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGD_2D_prj1Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGD_2D_prj1Character::TouchStopped);*/

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGD_2D_prj1Character::MoveRight);
	Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	// give the state a reference to the character
	IdleState->SetCharacter(this);

	// set the inital state to idle
	SetState(IdleState);
}

void AGD_2D_prj1Character::MoveRight(const FInputActionValue& Value)
{
	if (CurrentState)
	{
		CurrentState->MoveRight(Value.Get<float>());
	}
}

void AGD_2D_prj1Character::StartJump(const FInputActionValue& Value)
{
	if (CurrentState)
	{
		// jump the Player
		CurrentState->StartJump();
	}
}

void AGD_2D_prj1Character::StopJump(const FInputActionValue& Value)
{
	if (CurrentState)
	{
		// jump the Player
		StopJumping();
	}
}

void AGD_2D_prj1Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AGD_2D_prj1Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AGD_2D_prj1Character::UpdateCharacter()
{
	// Update animation to match the motion
	//UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AGD_2D_prj1Character::UpdateState()
{
	const FVector PlayerVelocity = GetVelocity();
	switch (CharacterState)
	{
	case ECharacterState::Idle:
		// from Idle we can run, jump or die horribly (if you added the health script), no dissapearing platforms so can't fall
		// jump has proity over run
		if (PlayerVelocity.Z > 0.1) // must be jumping
		{
			CharacterState = ECharacterState::Jumping;
		}
		else if (FMath::Abs(PlayerVelocity.X) > 0.1) // must be running
		{
			CharacterState = ECharacterState::Running;
		}
		break;
	case ECharacterState::Running:
		// From running we can idle, jump, fall or die
		// jump has proity over run
		if (PlayerVelocity.Z > 0.1) // must be jumping
		{
			CharacterState = ECharacterState::Jumping;
		}
		else if (PlayerVelocity.Z < -0.1) // must be falling
		{
			CharacterState = ECharacterState::Falling;
		}
		else if (FMath::Abs(PlayerVelocity.X) < 0.1) // must be idle
		{
			CharacterState = ECharacterState::Idle;
		}
		break;
	case ECharacterState::Jumping:
		// ether from jump to idle or fall
		if (PlayerVelocity.Z < -0.1) // must be falling
		{
			CharacterState = ECharacterState::Falling;
		}
		else if (PlayerVelocity.Z < 0.1) // must be idle
		{
			CharacterState = ECharacterState::Idle;
		}
		break;
	case ECharacterState::Falling:
		if (PlayerVelocity.Z > -0.1 && PlayerVelocity.Z < 0.1) // must have landed
		{
			CharacterState = ECharacterState::Idle;
		}
		break;
	case ECharacterState::Dead:
		// RIP
		break;
	}
}

void AGD_2D_prj1Character::HandleState()
{
	switch (CharacterState)
	{
	case ECharacterState::Idle:
		// Play idle animation
		UpdateAnimation(IdleAnimation);
		break;
	case ECharacterState::Running:
		// Play running animation
		UpdateAnimation(RunningAnimation);
		UpdateCharacter();
		break;
	case ECharacterState::Jumping:
		// Play jumping animation 
		UpdateAnimation(JumpingAnimation);
		UpdateCharacter();
		break;
	case ECharacterState::Falling:
		// Play falling animation
		UpdateAnimation(FallingAnimation);
		UpdateCharacter();
		break;
	case ECharacterState::Dead:
		// Play dead animation
		break;
	}
}

void AGD_2D_prj1Character::SetState(UCharacterState* NewState)
{
	// in reality you should implement the logic for enter state here
	// adjust the state
	CurrentState = NewState;
	CurrentState->EnterState();
	// in reality you should implement the logic for exit state here
}

UCharacterState* AGD_2D_prj1Character::GetCurrentState() const
{
	return CurrentState;
}
