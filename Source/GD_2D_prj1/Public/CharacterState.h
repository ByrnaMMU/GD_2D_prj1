// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IdleState.h" 
#include "PaperFlipbook.h" 
#include "UObject/NoExportTypes.h"
#include "CharacterState.generated.h"

class AGD_2D_prj1Character;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class GD_2D_PRJ1_API UCharacterState : public UObject
{
	GENERATED_BODY()
protected:
    // Make a reference so we can call character functionality
    UPROPERTY()
    AGD_2D_prj1Character* Character;
public:
    // the idle state of the player 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = States)
    class UCharacterState* IdleState;

    // State management 
    void SetState(UCharacterState* NewState);
    UCharacterState* GetCurrentState() const;

    // getter for the character
    void SetCharacter(AGD_2D_prj1Character* CharacterOwner);

    // Allow for unique behaviour when we enter and exit the state
    virtual void EnterState();
    virtual void ExitState();

    // notice we create all the function for the character here, as we still allow them to be called just can be ignored
    virtual void MoveRight(float Value);
    virtual void StartJump();
    virtual void StopJump();

    // our update and handle states
    virtual void UpdateState(float DeltaTime);
    virtual void HandleState();

    // We also handle overlap events in the case of iframes etc
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
