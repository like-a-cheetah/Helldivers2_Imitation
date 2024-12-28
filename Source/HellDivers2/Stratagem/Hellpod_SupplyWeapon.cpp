// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_SupplyWeapon.h"

AHellpod_SupplyWeapon::AHellpod_SupplyWeapon()
{
	AttachMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttachMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON.SK_HELLPOD_STRATAGEM_WEAPON'"));
	if (SkelMeshRef.Succeeded()) AttachMesh->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DefaultAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0x122126a000d3a390.SK_HELLPOD_STRATAGEM_WEAPON_A_0x122126a000d3a390'"));
	if (DefaultAnimRef.Object) DefaultAnim = DefaultAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SpreadAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0x9a24c46dbe8a9e32.SK_HELLPOD_STRATAGEM_WEAPON_A_0x9a24c46dbe8a9e32'"));
	if (SpreadAnimRef.Object) SpreadAnim = SpreadAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> FoldAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_WEAPON/SK_HELLPOD_STRATAGEM_WEAPON_A_0xf2fef7413421ec27.SK_HELLPOD_STRATAGEM_WEAPON_A_0xf2fef7413421ec27'"));
	if (FoldAnimRef.Object) FoldAnim = FoldAnimRef.Object;
}

void AHellpod_SupplyWeapon::BeginPlay()
{
	Super::BeginPlay();

	AttachMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AHellpod_SupplyWeapon::SpawnAttachment()
{
	Super::SpawnAttachment();

	AttachMesh->PlayAnimation(SpreadAnim, false);
}
