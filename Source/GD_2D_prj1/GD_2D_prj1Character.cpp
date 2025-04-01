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
#include "WalkState.h"
#include "JumpState.h"
#include "FallState.h"
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
	//GetSprite()->SetIsReplicated(true);
	//bReplicates = true;

	// create and attach the Idle state
	IdleState = CreateDefaultSubobject<UIdleState>(TEXT("IdleState"));
	// create and attach the Walk state
	WalkState = CreateDefaultSubobject<UWalkState>(TEXT("WalkState"));
	// create and attach the Jump state
	JumpState = CreateDefaultSubobject<UJumpState>(TEXT("JumpState"));
	// create and attach the Fall state
	FallState = CreateDefaultSubobject<UFallState>(TEXT("FallState"));

	// Bind the overlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGD_2D_prj1Character::OnOverlapBegin);
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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGD_2D_prj1Character::MoveRight);
	Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	FallState->SetCharacter(this);
	JumpState->SetCharacter(this);
	WalkState->SetCharacter(this);
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

void AGD_2D_prj1Character::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handle overlap with an   
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	if (OtherActor && (OtherActor != this))
	{
		// Print all tags of the other actor  
		for (const FName& Tag : OtherActor->Tags)
		{
			UE_LOG(LogTemp, Warning, TEXT("Collided with an object with tag: %s"), *Tag.ToString());
		}
		if (OtherActor->ActorHasTag("Enemy"))
		{
			FVector BounceDirection = GetActorLocation() - OtherActor->GetActorLocation();
			BounceDirection.Normalize();
			FVector Impulse = BounceDirection * 2500.0f;
			//push the player 
			GetCharacterMovement()->AddImpulse(Impulse, true);
		}
	}
}