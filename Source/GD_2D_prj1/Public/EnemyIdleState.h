// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyState.h"
#include "UObject/NoExportTypes.h"
#include "EnemyIdleState.generated.h"

class EnemyState;
/**
 * 
 */
UCLASS()
class GD_2D_PRJ1_API UEnemyIdleState : public UEnemyState
{
	GENERATED_BODY()
public:
    virtual void EnterState() override;
    virtual void MoveRight(float Value) override;
    //virtual void StartJump() override;
    virtual void HandleState() override;
};
