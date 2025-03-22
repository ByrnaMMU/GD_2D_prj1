// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "UObject/NoExportTypes.h"
#include "FallState.generated.h"

class CharacterState;
/**
 *
 */
UCLASS()
class GD_2D_PRJ1_API UFallState : public UCharacterState
{
    GENERATED_BODY()
public:
    virtual void EnterState() override;
    virtual void MoveRight(float Value) override;
    virtual void StartJump() override;
    virtual void HandleState() override;
};
