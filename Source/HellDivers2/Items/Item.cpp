// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Components/SphereComponent.h"

#include "Data/ItemData.h"
#include "Interface/CharacterItemInterface.h"

// Sets default values
AItem::AItem()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkelMeshComp->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	SkelMeshComp->SetSimulatePhysics(true);
	SkelMeshComp->SetCollisionProfileName(FName(TEXT("Item")));
	RootComponent = SkelMeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComponent"));
	SphereComp->InitSphereRadius(60.0f);  // Example values
	SphereComp->SetCollisionProfileName(FName(TEXT("Item")));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	SphereComp->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemData)
	{
		SetBaseData();
	}

}

void AItem::SetBaseData()
{
	ItemType = ItemData->ItemType;
	SkelMeshComp->SetSkeletalMeshAsset(ItemData->SkelMesh);
	InsertMontage = ItemData->InsertMontage;
	TakeOutMontage = ItemData->TakeOutMontage;
	SocketName = ItemData->SocketName;
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		//OverlappingPawn->TakeItem(this);
	}
}
