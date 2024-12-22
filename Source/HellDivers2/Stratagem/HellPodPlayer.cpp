// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/HellPodPlayer.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Characters/Player/PlayerCharacter.h"

AHellPodPlayer::AHellPodPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/SK_HELLPOD_PLAYER.SK_HELLPOD_PLAYER'"));
	if (SkelMeshRef.Succeeded()) SkelMeshComp->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> StartAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/Animations/SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83.SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83'"));
	if (StartAnimRef.Object) StartAnim = StartAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_RecallPlayerRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_RecallPlayer.MT_RecallPlayer'"));
	if (MT_RecallPlayerRef.Object) MT_RecallPlayer = MT_RecallPlayerRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ReadyToRandingRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_ReadyToRanding.MT_ReadyToRanding'"));
	if (MT_ReadyToRandingRef.Object) MT_ReadyToRanding = MT_ReadyToRandingRef.Object;

	CheckOnce = false;
}

void AHellPodPlayer::BeginPlay()
{
	Super::BeginPlay();

	SkelMeshComp->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, -10000.0f));

	if(StartAnim)
		SkelMeshComp->PlayAnimation(StartAnim, false);
}

void AHellPodPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckToLanding();
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

void AHellPodPlayer::CheckToLanding()
{
	FVector StartPos = GetActorLocation();
	StartPos.Z -= 500.0f;
	FVector EndPos = StartPos;
	EndPos.Z -= 5000.0f;
	float Radius = 50.0f;
	FHitResult OutHit;

	TArray<AActor*> Temp;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartPos,
		EndPos,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		Temp,
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true
	);

	if (bHit && !CheckOnce) {
		CheckOnce = true;

		SkelMeshComp->PlayAnimation(MT_ReadyToRanding, false);
		SkelMeshComp->SetLinearDamping(1.5f);
	}
}

