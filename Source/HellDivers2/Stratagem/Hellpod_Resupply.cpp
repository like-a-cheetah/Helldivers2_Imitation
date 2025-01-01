// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_Resupply.h"

#include "Items/Item.h"

AHellpod_Resupply::AHellpod_Resupply()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_SUPPLY/SK_HELLPOD_SUPPLY.SK_HELLPOD_SUPPLY'"));
	if (SkelMeshRef.Succeeded()) AttachMesh->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DefaultAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_SUPPLY/SK_HELLPOD_SUPPLY_Anim_0xb26eb3fc4f72bad6.SK_HELLPOD_SUPPLY_Anim_0xb26eb3fc4f72bad6'"));
	if (DefaultAnimRef.Object) DefaultAnim = DefaultAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SpreadAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_SUPPLY/SK_HELLPOD_SUPPLY_Anim_0x2a762c72db22004d.SK_HELLPOD_SUPPLY_Anim_0x2a762c72db22004d'"));
	if (SpreadAnimRef.Object) SpreadAnim = SpreadAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FoldAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_SUPPLY/SK_HELLPOD_SUPPLY_Anim_0x1695e6dfacb7c484.SK_HELLPOD_SUPPLY_Anim_0x1695e6dfacb7c484'"));
	if (FoldAnimRef.Object) FoldAnim = FoldAnimRef.Object;
}

void AHellpod_Resupply::BeginPlay()
{
	Super::BeginPlay();
}

void AHellpod_Resupply::SpawnSupplyItems()
{
	Super::SpawnSupplyItems();

	for (int i = 0; i < 4; i++)
	{
		AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(SupplyItem, SpawnParam);
		SpawnedItem->GetStaticMeshComp()->SetSimulatePhysics(false);

		FName SocketName = FName(FString::Printf(TEXT("ammo%d"), i));
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SpawnedItem->AttachToComponent(AttachMesh, AttachmentRules, SocketName);

		SupplyItems.Add(SpawnedItem);
	}
}
