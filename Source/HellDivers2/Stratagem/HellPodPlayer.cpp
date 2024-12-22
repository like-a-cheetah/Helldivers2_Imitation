// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/HellPodPlayer.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"

#include "Characters/Player/PlayerCharacter.h"

AHellPodPlayer::AHellPodPlayer() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/SK_HELLPOD_PLAYER.SK_HELLPOD_PLAYER'"));
	if (SkelMeshRef.Succeeded()) SkelMeshComp->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> StartAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/Animations/SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83.SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83'"));
	if (StartAnimRef.Object) StartAnim = StartAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_RecallPlayerRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_RecallPlayer.MT_RecallPlayer'"));
	if (MT_RecallPlayerRef.Object) MT_RecallPlayer = MT_RecallPlayerRef.Object;
}

void AHellPodPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(StartAnim)
		SkelMeshComp->PlayAnimation(StartAnim, false);
}

void AHellPodPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);

	SkelMeshComp->HideBoneByName(FName(TEXT("hatch")), EPhysBodyOp::PBO_None);
	SkelMeshComp->PlayAnimation(MT_RecallPlayer, false);
	SkelMeshComp->SetGenerateOverlapEvents(false);

	OnPlayerArrive.Execute();
}

void AHellPodPlayer::AttchPlayer(APlayerCharacter* Player)
{
	FAttachmentTransformRules CustomRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Player->AttachToActor(this, CustomRules, FName(TEXT("footrest_Socket")));

	OnPlayerArrive.BindUObject(Player, &APlayerCharacter::PlayerRebirth);
}