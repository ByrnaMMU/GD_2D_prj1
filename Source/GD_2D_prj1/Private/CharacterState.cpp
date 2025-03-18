// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterState.h"
#include "GD_2D_prj1/GD_2D_prj1Character.h"


void UCharacterState::SetCharacter(AGD_2D_prj1Character* CharacterOwner)
{
    Character = CharacterOwner;
    
}

void UCharacterState::EnterState() {}
void UCharacterState::ExitState() {}
void UCharacterState::MoveRight(float Value) {}
void UCharacterState::StartJump() {}
void UCharacterState::StopJump() {}
void UCharacterState::UpdateState(float DeltaTime) {}
void UCharacterState::HandleState() {}
void UCharacterState::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
