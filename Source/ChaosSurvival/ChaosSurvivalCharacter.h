// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DashCircleParticleSystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChaosSurvivalCharacter.generated.h"
UCLASS(Blueprintable)
class AChaosSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AChaosSurvivalCharacter();

	/** Primary Weapon **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCharacterWeapon* PrimaryWeapon;

	/** Dash Radius for RadiusToWorld as well as the dash ability **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDashCircleParticleSystem* DashCircleSystem;
	UFUNCTION()
		void ShowDashCircle();
	UFUNCTION()
		void HideDashCircle();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Gets Muzzle Location for LineTracing in PlayerController
	UFUNCTION()
		FVector GetMuzzleLocation();

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

