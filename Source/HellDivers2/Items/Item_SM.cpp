// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item_SM.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AItem_SM::AItem_SM()
{

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComp->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(FName(TEXT("Item")));
	SetRootComponent(StaticMeshComp);

	TiggerCollision->SetupAttachment(StaticMeshComp);
	InformWidget->SetupAttachment(StaticMeshComp);
}

void AItem_SM::BeginPlay()
{
	SkelMeshComp->DestroyComponent();
	SkelMeshComp = nullptr;

	Super::BeginPlay();
}

void AItem_SM::SetBaseData()
{
	Super::SetBaseData();

	if (ItemData->StaticMesh) StaticMeshComp->SetStaticMesh(ItemData->StaticMesh);
}

void AItem_SM::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	StaticMeshComp->SetOverlayMaterial(Mat_OutLiner);

	InformWidget->SetHiddenInGame(false);
}

void AItem_SM::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	StaticMeshComp->SetOverlayMaterial(nullptr);

	InformWidget->SetHiddenInGame(true);
}
