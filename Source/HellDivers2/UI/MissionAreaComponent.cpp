// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionAreaComponent.h"

#include "W_MissionArea.h"

void UMissionAreaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UW_MissionArea* MissionWidget = Cast<UW_MissionArea>(GetUserWidgetObject()))
		MissionWidget->SetWidgetComp(this);
}

UMissionAreaComponent::UMissionAreaComponent()
{
	SetWidgetSpace(EWidgetSpace::World);
	SetDrawSize(FVector2D(200, 200));

	bIsTwoSided = true;

	bReceiveHardwareInput = true;
}

void UMissionAreaComponent::SetInitTransform()
{
	USceneComponent* ParentComp = GetAttachParent();

	FVector Start = GetComponentLocation();
	FVector LineDir = ParentComp->GetComponentLocation() - Start;
	LineDir.Normalize();
	FVector End = Start + LineDir * 150.0f;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ECollisionChannel::ECC_WorldDynamic))
	{
		SetWorldLocation(Hit.ImpactPoint + Hit.Normal * 5.0f);
		SetWorldRotation(Hit.Normal.Rotation());
	}
}
