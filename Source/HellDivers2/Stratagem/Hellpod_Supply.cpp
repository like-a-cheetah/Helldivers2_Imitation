// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_Supply.h"

AHellpod_Supply::AHellpod_Supply()
{
	SetRootComponent(HellpodMesh);

	AttachMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachMesh"));

	SpawnParam.Owner = this;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AHellpod_Supply::BeginPlay()
{
	Super::BeginPlay();

	AttachMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	SpawnSupplyItems();
}

void AHellpod_Supply::SpawnAttachMachine()
{
	Super::SpawnAttachMachine();

	AttachMesh->SetCollisionProfileName(TEXT("BlockAll"));
	AttachMesh->PlayAnimation(SpreadAnim, false);
}

void AHellpod_Supply::FoldMachine()
{
	if(SupplyItems.Num() <= 0) AttachMesh->PlayAnimation(FoldAnim, false);
}

void AHellpod_Supply::SpawnSupplyItems()
{
}
