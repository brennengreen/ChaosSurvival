 // Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChaosSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DashCircleParticleSystem.h"
#include "CharacterWeapon.h"
#include "DrawDebugHelpers.h"



AChaosSurvivalCharacter::AChaosSurvivalCharacter()
{

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create weapon attachment
	PrimaryWeapon = CreateDefaultSubobject<UCharacterWeapon>(TEXT("PrimaryWeapon"));
	PrimaryWeapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> CursorDecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (CursorDecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(CursorDecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	   
	// Dash Radius for the decal and ability logic
	DashCircleSystem = CreateDefaultSubobject<UDashCircleParticleSystem>("DashCircleSystem");
	DashCircleSystem->SetupAttachment(RootComponent);
	DashCircleSystem->SetVisibility(false);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AChaosSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//InputComponent->BindAction("Dash", IE_Pressed, this, &AChaosSurvivalCharacter::ShowDashCircle);
	//InputComponent->BindAction("Dash", IE_Released, this, &AChaosSurvivalCharacter::HideDashCircle);

}

FVector AChaosSurvivalCharacter::GetMuzzleLocation()
{
	return FVector();
}

void AChaosSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("WeaponSocket"));
}

void AChaosSurvivalCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	/** This would work greate as the FireLine() Method
	FHitResult OutHit;
	FVector Start = GetActorLocation();

	FVector ForwardVector = GetActorForwardVector();

	FHitResult CursorTraceHitResult;
	Cast<APlayerController>(GetController())->GetHitResultUnderCursor(ECC_Visibility, true, CursorTraceHitResult);
	FVector End = (CursorTraceHitResult.Location);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);
	if (ActorLineTraceSingle(OutHit, Start, End, ECC_WorldStatic, CollisionParams)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("The component being hit is: %s"), *OutHit.GetComponent()->GetName());
	} **/
}

void AChaosSurvivalCharacter::ShowDashCircle()
{
	if (DashCircleSystem != nullptr)
	{
		DashCircleSystem->SetVisibility(true);
	}
}

void AChaosSurvivalCharacter::HideDashCircle()
{
	if (DashCircleSystem != nullptr)
	{
		DashCircleSystem->SetVisibility(false);
	}
}
