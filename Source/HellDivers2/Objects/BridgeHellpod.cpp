// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BridgeHellpod.h"

#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "Interface/PlayerControl.h"

FOnSetupBridgeHellpods ABridgeHellpod::OnSetupBridgeHellpods;

ABridgeHellpod::ABridgeHellpod()
{
	SkelMeshComp->SetSimulatePhysics(false);
	SkelMeshComp->SetCollisionProfileName(TEXT("BlockAll"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/PROPS/SKELETAL_MESH/HELLPOD/SK_BRIDGE_HELLPOD.SK_BRIDGE_HELLPOD'"));
	if (SkelMeshRef.Succeeded()) SkelMeshComp->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AS_IdleRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/HELLPOD/Animation/A_0xf1b854d5e35d6a4b.A_0xf1b854d5e35d6a4b'"));
	if (AS_IdleRef.Object) AS_ReadyIdle = AS_IdleRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AS_ReadyIdleRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/HELLPOD/Animation/A_0xa905f0296b200f93_POSE.A_0xa905f0296b200f93_POSE'"));
	if (AS_ReadyIdleRef.Object) AS_ReadyIdle = AS_ReadyIdleRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_SetupRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_Setup.MT_Setup'"));
	if (MT_SetupRef.Object) MT_Setup = MT_SetupRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ResetRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_Reset.MT_Reset'"));
	if (MT_ResetRef.Object) MT_Reset = MT_ResetRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_HellpodReadyRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_HellpodReady.MT_HellpodReady'"));
	if (MT_HellpodReadyRef.Object) MT_HellpodReady = MT_HellpodReadyRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_LockHellpodRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_LockHellpod.MT_LockHellpod'"));
	if (MT_LockHellpodRef.Object) MT_LockHellpod = MT_LockHellpodRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_UnlockHellpodRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_UnlockHellpod.MT_UnlockHellpod'"));
	if (MT_UnlockHellpodRef.Object) MT_UnlockHellpod = MT_UnlockHellpodRef.Object;

	SkelMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	TiggerCollision->SetRelativeLocation(FVector(0.0, -165.0, 76.0));
	TiggerCollision->InitSphereRadius(200);

	OnSetupBridgeHellpods.AddUObject(this, &ABridgeHellpod::SetActiveOverlapEvent);
}

void ABridgeHellpod::BeginPlay()
{
	Super::BeginPlay();

	SkelMeshComp->SetAnimation(AS_Idle);

	ALevelSequenceActor* OutActor = nullptr;

	FMovieSceneSequencePlaybackSettings PlaybackSet;
	PlaybackSet.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	PlaybackSet.bPauseAtEnd = true;

	ULevelSequencePlayer* LSPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LS_ToStratagemSetting,
		PlaybackSet,
		OutActor
	);

	LSActor = OutActor;

	SetActiveOverlapEvent(false);
}

void ABridgeHellpod::Interact_Implementation(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		User = Actor;

		SkelMeshComp->PlayAnimation(MT_LockHellpod, false);
		Player->EnterHellpodBridge(this);

		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [this]() {
				LSActor->SequencePlayer->Play();
			}, 4.5f, false);

		LSActor->SequencePlayer->Play();
		LSActor->SequencePlayer->Pause();

		Player->SetCurrentLVSequenceActor(LSActor);
	}
}

void ABridgeHellpod::Escape_Implementation(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(User);
	if (Player)
	{
		SkelMeshComp->PlayAnimation(MT_UnlockHellpod, false);

		FTimerHandle Timer1;
		GetWorld()->GetTimerManager().SetTimer(Timer1, [this, Player]() {
			Player->LeaveHellpodBridge(this);
			}, 1.8f, false);

		FTimerHandle Timer2;
		GetWorld()->GetTimerManager().SetTimer(Timer2, [this]() { SetActiveOverlapEvent(true); }, MT_UnlockHellpod->GetPlayLength(), false);

		User = nullptr;
	}
}

void ABridgeHellpod::SetActiveOverlapEvent(bool bActive)
{
	if (UWorld* World = GetWorld())
	{
		if (bActive)
		{
			float EndTime = MT_Setup->GetPlayLength();

			SkelMeshComp->PlayAnimation(MT_Setup, false);

			FTimerHandle Timer;
			World->GetTimerManager().SetTimer(Timer,
				[this]() { Super::SetActiveOverlapEvent(true); },
				EndTime, false);
		}
		else
		{
			Super::SetActiveOverlapEvent(bActive);

			if(!User) SkelMeshComp->PlayAnimation(MT_Reset, false);
		}
	}
}
