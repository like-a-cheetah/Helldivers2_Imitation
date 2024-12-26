// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BridgeHellpod.h"

#include "Components/BoxComponent.h"

#include "Interface/PlayerControl.h"

ABridgeHellpod::ABridgeHellpod()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(SkelMeshComp);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/PROPS/SKELETAL_MESH/HELLPOD/SK_BRIDGE_HELLPOD.SK_BRIDGE_HELLPOD'"));
	if (SkelMeshRef.Succeeded()) SkelMeshComp->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AS_IdleRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/PROPS/SKELETAL_MESH/HELLPOD/Animation/A_0xf1b854d5e35d6a4b.A_0xf1b854d5e35d6a4b'"));
	if (AS_IdleRef.Object) AS_Idle = AS_IdleRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_HellpodReadyRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_HellpodReady.MT_HellpodReady'"));
	if (MT_HellpodReadyRef.Object) MT_HellpodReady = MT_HellpodReadyRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_LockHellpodRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Objects/ShipInterior/BridgeHellpod/MT_LockHellpod.MT_LockHellpod'"));
	if (MT_LockHellpodRef.Object) MT_LockHellpod = MT_LockHellpodRef.Object;

	SkelMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SkelMeshComp->SetAnimation(AS_Idle);
	
	BoxCol = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollider"));
	BoxCol->SetupAttachment(RootComponent);

	BoxCol->SetRelativeLocation(FVector(0.0f, 96.0f, 0.0f));
	BoxCol->SetBoxExtent(FVector(160.0f, 32.0f, 32.0f), true);
	BoxCol->SetCollisionProfileName(TEXT("Trigger"));
	BoxCol->SetSimulatePhysics(false);
	BoxCol->SetGenerateOverlapEvents(true);
	BoxCol->OnComponentBeginOverlap.AddDynamic(this, &ABridgeHellpod::OnOverlapBegin);
}

void ABridgeHellpod::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABridgeHellpod::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	IPlayerControl* Player = Cast<IPlayerControl>(OtherActor);
	if(Player)
	{
		Player->SetNearbyInteractable(this);
	}
}

void ABridgeHellpod::Interact(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		SkelMeshComp->PlayAnimation(MT_LockHellpod, false);

		Player->EnterHellpodBridge();
	}
}
