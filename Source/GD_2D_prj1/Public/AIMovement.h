// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/ActorComponent.h"
#include "AIMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GD_2D_PRJ1_API UAIMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Array of patrol nodes for the AI to follow, we will simply ping pong between these nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Pathfinding")
	TArray<AActor*> PatrolNodes;


private:
	int CurrentPatrolNodeIndex = 0; // track our current node
	AEnemy* OwnerPawn; // store our owner
	void MoveToNextPatrolNode(); // calculate movement

		
};
