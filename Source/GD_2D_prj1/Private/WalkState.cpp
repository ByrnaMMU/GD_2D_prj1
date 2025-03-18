// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkState.h"
#include "GD_2D_prj1/GD_2D_prj1Character.h"

void UWalkState::EnterState()
{
    if (Character)
    {
        // Play idle animation
        Character->UpdateAnimation(Character->RunningAnimation);
    }
}

// add movement to the player
void UWalkState::MoveRight(float Value)
{
    // note is some we would remove the handle state and just run from here the change
    if (Character)
    {
        // Apply the input to the character motion
        Character->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    }
}

void UWalkState::StartJump()
{
    if (Character)
    {
        Character->Jump();
    }
}

void UWalkState::HandleState()
{
    if (Character)
    {
        // get the velocity of the player
        const FVector PlayerVelocity = Character->GetVelocity();

        if (FMath::Abs(PlayerVelocity.Z) > 0.1) // must be jumping
        {
            // switch to jump state
        }
        else if (FMath::Abs(PlayerVelocity.X) > 0.1) // must be running
        {
            // switch to walk state
        }
    }
}