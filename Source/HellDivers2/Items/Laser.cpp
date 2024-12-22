// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Laser.h"

#include "DrawDebugHelpers.h"

// Sets default values
ALaser::ALaser()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HellDivers2/Weapons/LaserMaterial/StaticMesh.StaticMesh'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start;
	FRotator DirectRot;
	Mesh->GetSocketWorldLocationAndRotation(TEXT("LaserHole"), Start, DirectRot);
	FVector End = Start + DirectRot.Vector() * 5000.0f;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.01f, 0, 1.0f);
}
