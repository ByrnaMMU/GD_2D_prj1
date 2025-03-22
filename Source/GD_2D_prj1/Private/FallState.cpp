// Fill out your copyright notice in the Description page of Project Settings.


#include "FallState.h"
#include "GD_2D_prj1/GD_2D_prj1Character.h"

void UFallState::EnterState()
{
    if (Character)
    {
        // Play idle animation
        Character->UpdateAnimation(Character->FallingAnimation);
    }
}

// add movement to the player
void UFallState::MoveRight(float Value)
{
    // note is some we would remove the handle state and just run from here the change
    if (Character)
    {
        // Apply the input to the character motion
        Character->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
    }
}

void UFallState::StartJump()
{
    if (Character)
    {
        Character->Jump();
    }
}

void UFallState::HandleState()
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

        if (FMath::Abs(PlayerVelocity.Z) < 0.09) // must have landed
        {
            if (FMath::Abs(PlayerVelocity.X) < 0.1) // must be idle
            {
                // switch to idle state
                Character->SetState(Character->IdleState);
            }
            else if (FMath::Abs(PlayerVelocity.X) > 0.1) // must be walking
            {
                // switch to idle state
                Character->SetState(Character->WalkState);
            }
        }
        
    }
}