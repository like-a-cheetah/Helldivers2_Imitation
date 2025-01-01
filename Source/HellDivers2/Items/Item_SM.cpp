// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item_SM.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AItem_SM::AItem_SM()
{
	SkelMeshComp->DestroyComponent();

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(FName(TEXT("Item")));
	SetRootComponent(StaticMeshComp);

	SphereComp->SetupAttachment(StaticMeshComp);
	InformWidget->SetupAttachment(StaticMeshComp);
}

void AItem_SM::SetBaseData()
{
	Super::SetBaseData();

	if (ItemData->StaticMesh) StaticMeshComp->SetStaticMesh(ItemData->StaticMesh);
}
