// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyState.generated.h"

class AEnemy;

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class GD_2D_PRJ1_API UEnemyState : public UObject
{
    GENERATED_BODY()
protected:
    // Make a reference so we can call character functionality
    UPROPERTY()
    AEnemy* Character;
public:
    // getter for the character
    void SetCharacter(AEnemy* CharacterOwner);

    // Allow for unique behaviour when we enter and exit the state
    virtual void EnterState();
    virtual void ExitState();

    // notice we create all the function for the character here, as we still allow them to be called just can be ignored
    virtual void MoveRight(float Value);
    //virtual void StartJump();
    //virtual void StopJump();

    // our update and handle states
    virtual void UpdateState(float DeltaTime);
    virtual void HandleState();

};

