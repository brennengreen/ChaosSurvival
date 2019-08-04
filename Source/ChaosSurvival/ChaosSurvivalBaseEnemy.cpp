// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaosSurvivalBaseEnemy.h"

// Sets default values
AChaosSurvivalBaseEnemy::AChaosSurvivalBaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChaosSurvivalBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChaosSurvivalBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChaosSurvivalBaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

