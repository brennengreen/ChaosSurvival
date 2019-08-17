// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChaosSurvivalPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ChaosSurvivalCharacter.h"
#include "Engine/World.h"
/////////////////////////////////////////////////////////////////////////////
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "DrawDebugHelpers.h"
/////////////////////////////////////////////////////////////////////////////


AChaosSurvivalPlayerController::AChaosSurvivalPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	/** Dash Configurations **/
	fDashRadius = 400.f;
	fDashCooldown = 3.0f;
	fDashVelocity = 3500.f;
	bCanDash = true;
}

void AChaosSurvivalPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
	if (bIsShooting)
	{
		Shoot();
	}
}

void AChaosSurvivalPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AChaosSurvivalPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AChaosSurvivalPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AChaosSurvivalPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AChaosSurvivalPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AChaosSurvivalPlayerController::OnResetVR);

	// Dash
	InputComponent->BindAction("Dash", IE_Pressed, this, &AChaosSurvivalPlayerController::OnDashDown);
	InputComponent->BindAction("Dash", IE_Released, this, &AChaosSurvivalPlayerController::OnDashReleased);

	// Shoot
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AChaosSurvivalPlayerController::OnShootDown);
	InputComponent->BindAction("Shoot", IE_Released, this, &AChaosSurvivalPlayerController::OnShootReleased);
}

void AChaosSurvivalPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AChaosSurvivalPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AChaosSurvivalCharacter* MyPawn = Cast<AChaosSurvivalCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AChaosSurvivalPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AChaosSurvivalPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AChaosSurvivalPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AChaosSurvivalPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AChaosSurvivalPlayerController::OnDashDown()
{
	AChaosSurvivalCharacter* MyCharacter = Cast<AChaosSurvivalCharacter>(GetPawn());

	if (bCanDash)
	{
		MyCharacter->ShowDashCircle();
	}
}

void AChaosSurvivalPlayerController::OnDashReleased()
{
	AChaosSurvivalCharacter* MyCharacter = Cast<AChaosSurvivalCharacter>(GetPawn());
	MyCharacter->HideDashCircle();

	if (bCanDash)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		APawn* const MyPawn = GetPawn();
		FVector CurrentLocation = MyPawn->GetActorLocation();

		float Distance = FVector::Dist(Hit.ImpactPoint, CurrentLocation);
		if (Distance <= fDashRadius)
		{
			bCanDash = false;

			// Get the current FVector Location in the world of the cursor
			FVector HitLocation = FVector(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, 1);
			// Subtract cursor location by actor location to the the resultant
			// FVector for proper trajectory
			FVector LaunchVector = FVector(HitLocation - CurrentLocation);

			UCharacterMovementComponent* CharacterMovement = MyCharacter->GetCharacterMovement();
			CharacterMovement->StopMovementImmediately();
			CharacterMovement->BrakingFrictionFactor = 0.f;
			MyCharacter->LaunchCharacter(LaunchVector.GetSafeNormal() * fDashVelocity, true, true);

			GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &AChaosSurvivalPlayerController::StopDashing, 0.1f, false);
		}
	}
}

void AChaosSurvivalPlayerController::StopDashing()
{
	Cast<AChaosSurvivalCharacter>(GetPawn())->GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(UnusedTimerHandle, this, &AChaosSurvivalPlayerController::ResetDash, fDashCooldown, false);
}

void AChaosSurvivalPlayerController::ResetDash()
{
	bCanDash = true;
}


void AChaosSurvivalPlayerController::OnShootDown()
{
	bIsShooting = true;
}

void AChaosSurvivalPlayerController::OnShootReleased()
{
	bIsShooting = false;
}

void AChaosSurvivalPlayerController::Shoot()
{
	// Get Location of Muzzle and Cursor, Subtract for Unit Vector
	// Then LineTrace() in that direction

	// Trace Line then ProcessHit() anything that is hit
	// If ProcessHit() returns true, line hit enemy
	// So deal damage accordingly

	UE_LOG(LogTemp, Display, TEXT("Pew"));
}

bool AChaosSurvivalPlayerController::ProcessHit(class UObject* HitActor)
{
	// If Actor is EnemyAI then return true else false
	return false;
}
