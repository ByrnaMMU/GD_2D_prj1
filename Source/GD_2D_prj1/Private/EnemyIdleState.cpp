// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyIdleState.h"
#include "Enemy.h"

void UEnemyIdleState::EnterState()
{
    if (Character)
    {
        // Play idle animation
        Character->UpdateAnimation(Character->IdleAnimation);
    }
}

// add movement to the player
void UEnemyIdleState::MoveRight(float Value)
{
    // note is some we would remove the handle state and just run from here the change
    if (Character)
    {
        // Apply the input to the character motion
        Character->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);

    }
}

//void UEnemyIdleState::StartJump()
//{
//    if (Character)
//    {
//        Character->Jump();
//    }
//}

void UEnemyIdleState::HandleState()
{
    if (Character)
    {
        // get the velocity of the Enemy
        const FVector EnemyVelocity = Character->GetVelocity();
        float TravelDirection = EnemyVelocity.X;
        // Set the rotation so that the character faces his direction of travel.
        if (Character->Controller != nullptr)
        {
            if (TravelDirection < 0.0f)
            {
                Character->Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
            }
            else if (TravelDirection > 0.0f)
            {
                Character->Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
            }
        }

        //if (FMath::Abs(EnemyVelocity.Z) > 0.1) // must be jumping
        //{
        //    if (EnemyVelocity.Z > 0.1)
        //    {
        //        // switch to jump state
        //        Character->SetState(Character->JumpState);
        //    }
        //    else if (EnemyVelocity.Z < 0.1)
        //    {
        //        // switch to jump state
        //        Character->SetState(Character->FallState);
        //    }
        //}
        if (FMath::Abs(EnemyVelocity.X) > 0.1) // must be running
        {
            // switch to walk state
            //Character->SetState(Character->EnemyWalkState);
        }
    }
}