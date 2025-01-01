// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

#include "Data/ItemData.h"
#include "Interface/PlayerControl.h"

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
	SphereComp->SetCollisionProfileName(FName(TEXT("Trigger")));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	SphereComp->SetupAttachment(RootComponent);

	InformWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	InformWidget->SetupAttachment(SkelMeshComp);

	static ConstructorHelpers::FClassFinder<UUserWidget> InformWidgetRef(TEXT("/Game/HellDivers2/UI/WBP_ObjectInformWidget.WBP_ObjectInformWidget_C"));
	InformWidget->SetWidgetClass(InformWidgetRef.Class);
	InformWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InformWidget->SetDrawSize(FVector2D(150.0f, 15.0f));
	InformWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InformWidget->SetHiddenInGame(true);
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
	if(ItemData->SkelMesh) SkelMeshComp->SetSkeletalMeshAsset(ItemData->SkelMesh);
	InsertMontage = ItemData->InsertMontage;
	TakeOutMontage = ItemData->TakeOutMontage;
	SocketName = ItemData->SocketName;
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IPlayerControl* Player = Cast<IPlayerControl>(OtherActor);
	if (Player)
	{
		InformWidget->SetHiddenInGame(false);
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPlayerControl* Player = Cast<IPlayerControl>(OtherActor);
	if (Player)
	{
		InformWidget->SetHiddenInGame(true);
	}
}
