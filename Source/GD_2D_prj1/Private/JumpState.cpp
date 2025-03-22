// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpState.h"
#include "GD_2D_prj1/GD_2D_prj1Character.h"

void UJumpState::EnterState()
{
    if (Character)
    {
        // Play idle animation
        Character->UpdateAnimation(Character->JumpingAnimation);
    }
}

// add movement to the player
void UJumpState::MoveRight(float Value)
{
    // note is some we would remove the handle state and just run from here the change
    if (Character)
    {
        // Apply the input to the character motion
        Character->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    }
}

void UJumpState::StartJump()
{
    if (Character)
    {
        Character->Jump();
    }
}

void UJumpState::HandleState()
{
    if (Character)
    {
        // get the velocity of the player
        const FVector PlayerVelocity = Character->GetVelocity();
        float TravelDirection = PlayerVelocity.X;
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

        if (PlayerVelocity.Z < 0.1) // must be falling (or finished jumping)
        {
            // switch to fall state
            Character->SetState(Character->FallState);
        }
    }
}