// Fill out your copyright notice in the Description page of Project Settings.


#include "DashCircleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

UDashCircleComponent::UDashCircleComponent()
{
	// Setting the Static Mesh to the basic plane shape
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Geometry/Plane.Plane'"));
	if (MeshAsset.Succeeded())
	{
		UStaticMesh* Asset = MeshAsset.Object;
		UDashCircleComponent::SetStaticMesh(Asset);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset(TEXT("Material'/Game/Materials/PlayerMaterials/DashCircle.DashCircle'"));
	if (MaterialAsset.Succeeded())
	{
		UMaterial* Asset = MaterialAsset.Object;
		UStaticMesh* StaticMeshPtr = UDashCircleComponent::GetStaticMesh();
		StaticMeshPtr->SetMaterial(0, Asset);
	}

}