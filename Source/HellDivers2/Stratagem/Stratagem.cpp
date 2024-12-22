// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Stratagem.h"

AStratagem::AStratagem()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkelMeshComp->SetCollisionProfileName(TEXT("Stratagem"));
	SkelMeshComp->SetSimulatePhysics(true);
	SkelMeshComp->SetGenerateOverlapEvents(true);
	SkelMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AStratagem::OnOverlapBegin);
	SkelMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void AStratagem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	SkelMeshComp->SetSimulatePhysics(false);

	FVector DropPoint = GetActorLocation();
	DropPoint.Z = SweepHitResult.ImpactPoint.Z;

	SetActorLocation(DropPoint);
}

