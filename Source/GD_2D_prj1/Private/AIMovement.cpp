// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMovement.h"

// Sets default values for this component's properties
UAIMovement::UAIMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentPatrolNodeIndex = 0;
}


// Called when the game starts
void UAIMovement::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<AEnemy>(GetOwner());
	if (!OwnerPawn) // if the owner of this component is null
	{
		UE_LOG(LogTemp, Error, TEXT("AIMovementComponent can only be attached to a Pawn."));
		this->DestroyComponent();
	}
}


// Called every frame
void UAIMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveToNextPatrolNode();
	// ...
}

void UAIMovement::MoveToNextPatrolNode()
{
	
	// If we have no patrol nodes, return
	if (PatrolNodes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("no nodes"));
		return;
	}
	else if (PatrolNodes.Num() == 1) // If we have only one patrol node, set the owner's location to that node's location
	{
		UE_LOG(LogTemp, Warning, TEXT("one node"));
		OwnerPawn->SetActorLocation(PatrolNodes[0]->GetActorLocation());
		return;
	}
	else 
	{
		if (OwnerPawn->GetActorLocation().X > PatrolNodes[CurrentPatrolNodeIndex]->GetActorLocation().X) // are we moving left or right?
		{
			// move left
			//UE_LOG(LogTemp, Warning, TEXT("go left"));
			OwnerPawn->MoveRight(-1.0f);

		}
		else
		{
			// move right
			//UE_LOG(LogTemp, Warning, TEXT("go right"));
			OwnerPawn->MoveRight(1.0f);

		}

		if ((FMath::Abs(OwnerPawn->GetActorLocation().X - PatrolNodes[CurrentPatrolNodeIndex]->GetActorLocation().X)) < 0.5f)
		{
			CurrentPatrolNodeIndex++;
			UE_LOG(LogTemp, Warning, TEXT("Patrol Index: %d"), CurrentPatrolNodeIndex);
		}

		if (CurrentPatrolNodeIndex >= PatrolNodes.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("reset count"));
			CurrentPatrolNodeIndex = 0;

		}
	}

	
	// check if we have reached the patrol node
}
