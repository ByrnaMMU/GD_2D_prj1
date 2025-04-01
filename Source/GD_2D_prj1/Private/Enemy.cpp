// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PaperFlipbookComponent.h"
//#include "PaperSpriteComponent.h"
#include "EnemyIdleState.h"
#include "PaperFlipbook.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	//RootComponent = SpriteComponent;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);

	// create and attach the Idle state
	EnemyIdleState = CreateDefaultSubobject<UEnemyIdleState>(TEXT("EnemyIdleState"));
	// create and attach the Walk state
	//WalkState = CreateDefaultSubobject<UWalkState>(TEXT("WalkState"));

	// Enable replication on the Sprite component so animations show up when networked
	//GetSprite()->SetIsReplicated(true);
	//bReplicates = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//UpdateAnimation(IdleAnimation);
}

void AEnemy::UpdateAnimation(UPaperFlipbook* animation)
{
	// Are we playing this animation?
	if (GetSprite()->GetFlipbook() != animation)
	{
		GetSprite()->SetFlipbook(animation);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EnemyIdleState->SetCharacter(this);
	// set the inital state to idle
	SetState(EnemyIdleState);
}

void AEnemy::MoveRight(const FInputActionValue& Value)
{
	// Apply the input to the character motion
	SetActorLocation(GetActorLocation() + FVector(1.0f, 0.0f, 0.0f) * Value.Get<float>()); // this is different but some reason the normal way failed. 
}

void AEnemy::SetState(UEnemyState* NewState)
{
	// in reality you should implement the logic for enter state here
	// adjust the state
	CurrentState = NewState;
	CurrentState->EnterState();
	// in reality you should implement the logic for exit state here
}

UEnemyState* AEnemy::GetCurrentState() const
{
	return CurrentState;
}