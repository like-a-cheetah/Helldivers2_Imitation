// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Data/ItemData.h"
#include "Interface/PlayerControl.h"

AItem::AItem()
{
	SkelMeshComp->SetSimulatePhysics(true);

	TiggerCollision->InitSphereRadius(150.f);

	SetActiveOverlapEvent(false);

	Description = FText().FromString(TEXT("사용"));

	SkelMeshComp->SetSimulatePhysics(true);
	TiggerCollision->SetGenerateOverlapEvents(true);

	OnPickedUp.BindLambda([this](bool bPickedUp) {
		SkelMeshComp->SetSimulatePhysics(!bPickedUp);
		if(TiggerCollision) TiggerCollision->SetGenerateOverlapEvents(!bPickedUp);
	});
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemData)
	{
		SetBaseData();
	}

	if (Owner) OnPickedUp.Execute(true);
	else OnPickedUp.Execute(false);
}

void AItem::SetBaseData()
{
	ItemType = ItemData->ItemType;
	if(ItemData->SkelMesh) SkelMeshComp->SetSkeletalMeshAsset(ItemData->SkelMesh);
	InsertMontage = ItemData->InsertMontage;
	TakeOutMontage = ItemData->TakeOutMontage;
	SocketName = ItemData->SocketName;
}

void AItem::Throw(FVector Force)
{
	SkelMeshComp->SetSimulatePhysics(true);
	SkelMeshComp->AddImpulse(Force);
}

//void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
//{	
//	//PlayerControl* Player = Cast<IPlayerControl>(OtherActor);
//	// (Player)
//	{
//		//InformWidget->SetHiddenInGame(false);
//	}
//}
//
//void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	//IPlayerControl* Player = Cast<IPlayerControl>(OtherActor);
//	//if (Player)
//	{
//		//InformWidget->SetHiddenInGame(true);
//	}
//}
