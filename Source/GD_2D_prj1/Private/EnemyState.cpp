//// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyState.h"
#include "Enemy.h"


void UEnemyState::SetCharacter(AEnemy* CharacterOwner)
{
    Character = CharacterOwner;
}

void UEnemyState::EnterState() {}
void UEnemyState::ExitState() {}
void UEnemyState::MoveRight(float Value) {}
//void UEnemyState::StartJump() {}
//void UEnemyState::StopJump() {}
void UEnemyState::UpdateState(float DeltaTime) {}
void UEnemyState::HandleState() {}
