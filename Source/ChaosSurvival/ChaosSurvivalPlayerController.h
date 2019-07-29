// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChaosSurvivalPlayerController.generated.h"
UCLASS()
class AChaosSurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChaosSurvivalPlayerController();

	/** Action handlers for Dashing **/
	UFUNCTION()
		void OnDashDown();
	UFUNCTION()
		void OnDashReleased();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float fDashRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float fDashVelocity;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};