// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
//#include "PaperSpriteComponent.h"
#include "EnemyIdleState.h"
//#include "GameFramework/Pawn.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbook.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeathAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HitAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* ReactAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation(UPaperFlipbook* animation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	//UPaperSpriteComponent* SpriteComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UBoxComponent* Collider;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// enable the Enemy to move
	void MoveRight(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UEnemyState* CurrentState;
	// the idle state of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
	class UEnemyState* EnemyIdleState;

	// State management
	void SetState(UEnemyState* NewState);
	UEnemyState* GetCurrentState() const;
};