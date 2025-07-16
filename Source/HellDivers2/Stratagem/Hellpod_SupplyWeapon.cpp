// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_SupplyWeapon.h"

#include "Items/Item.h"

//AHellpod_SupplyWeapon::AHellpod_SupplyWeapon()
//{
//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON.SK_HELLPOD_STRATAGEM_WEAPON'"));
//	if (SkelMeshRef.Succeeded()) AttachMesh->SetSkeletalMeshAsset(SkelMeshRef.Object);
//
//	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DefaultAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0x122126a000d3a390.SK_HELLPOD_STRATAGEM_WEAPON_A_0x122126a000d3a390'"));
//	if (DefaultAnimRef.Object) DefaultAnim = DefaultAnimRef.Object;
//
//	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SpreadAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0x9a24c46dbe8a9e32.SK_HELLPOD_STRATAGEM_WEAPON_A_0x9a24c46dbe8a9e32'"));
//	if (SpreadAnimRef.Object) SpreadAnim = SpreadAnimRef.Object;
//
//	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FoldAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0xf2fef7413421ec27.SK_HELLPOD_STRATAGEM_WEAPON_A_0xf2fef7413421ec27'"));
//	if (FoldAnimRef.Object) FoldAnim = FoldAnimRef.Object;
//
//}
//
//void AHellpod_SupplyWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
//void AHellpod_SupplyWeapon::SpawnSupplyItems()
//{
//	Super::SpawnSupplyItems();
//
//	AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(SupplyItem, SpawnParam);
//	SpawnedItem->SetActiveOverlapEvent(true);
//	//SpawnedItem->GetSkelMeshComp()->SetSimulatePhysics(false);
//	SpawnedItem->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("attach0"));
//
//	SupplyItems.Add(SpawnedItem);
//
//	if (SubItem)
//	{
//		SpawnedItem = GetWorld()->SpawnActor<AItem>(SubItem);
//		//SpawnedItem->GetSkelMeshComp()->SetSimulatePhysics(false);
//		SpawnedItem->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("attach1"));
//	}
//	else
//	{
//		SpawnedItem = GetWorld()->SpawnActor<AItem>(SupplyItem);
//		//SpawnedItem->GetSkelMeshComp()->SetSimulatePhysics(false);
//		SpawnedItem->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("attach1"));
//	}
//	SupplyItems.Add(SpawnedItem);
//}

AHellpod_SupplyWeapon::AHellpod_SupplyWeapon()
{
}
