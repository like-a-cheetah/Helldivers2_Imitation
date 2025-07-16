// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Data/ItemData.h"
#include "Interface/PlayerControl.h"
#include "Characters/Player/PlayerCharacter.h"

AItem::AItem()
{
	SkelMeshComp->SetSimulatePhysics(true);

	TiggerCollision->InitSphereRadius(150.f);

	SetActiveOverlapEvent(false);

	Description = FText().FromString(TEXT("사용"));

	SkelMeshComp->SetSimulatePhysics(true);
	TiggerCollision->SetGenerateOverlapEvents(true);

	OnPickedUp.BindUObject(this, &AItem::AttachToPlayer);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemData)
	{
		SetBaseData();
	}

	if (Owner)
	{
		if(Cast<APlayerCharacter>(Owner)) OnPickedUp.Execute(true);
		else
		{
			if (SkelMeshComp) SkelMeshComp->SetSimulatePhysics(false);
			else if(StaticMeshComp) StaticMeshComp->SetSimulatePhysics(false);
			if (TiggerCollision) 
				TiggerCollision->SetGenerateOverlapEvents(true);
		}
	}
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

void AItem::AttachToPlayer(bool bPickedUp)
{
	if (SkelMeshComp) SkelMeshComp->SetSimulatePhysics(!bPickedUp);
	else if (StaticMeshComp) StaticMeshComp->SetSimulatePhysics(!bPickedUp);
	if (TiggerCollision) TiggerCollision->SetGenerateOverlapEvents(!bPickedUp);

	if (!bPickedUp)
	{
		SetOwner(nullptr);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		OnDetachFromSupplyHellpod.Broadcast();
	}
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
