// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DashCircleComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChaosSurvivalCharacter.generated.h"
UCLASS(Blueprintable)
class AChaosSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AChaosSurvivalCharacter();

	/** Dash Radius for RadiusToWorld as well as the dash ability **/
	UPROPERTY(VisibleAnywhere)
		class UDashCircleComponent* DashCircle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DashRadius;
	UFUNCTION()
		void ShowDashCircle();
	UFUNCTION()
		void HideDashCircle();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;
};

