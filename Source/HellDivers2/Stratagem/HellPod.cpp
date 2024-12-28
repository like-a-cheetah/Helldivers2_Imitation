// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/InputComponent.h"

AHellpod::AHellpod() 
{
	PrimaryActorTick.bCanEverTick = true;

	HellpodMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HellpodMesh"));
	HellpodMesh->SetCollisionProfileName(TEXT("Hellpod"));
	HellpodMesh->SetSimulatePhysics(true);
	HellpodMesh->SetGenerateOverlapEvents(true);
	HellpodMesh->OnComponentBeginOverlap.AddDynamic(this, &AHellpod::OnMeshOverlapBegin);
	HellpodMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	HellpodMesh->SetAllMassScale(10.0f);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(HellpodMesh);
	BoxCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHellpod::OnBoxOverlapBegin);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/SK_HELLPOD_PLAYER.SK_HELLPOD_PLAYER'"));
	if (SkelMeshRef.Succeeded()) HellpodMesh->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> StartAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/Animations/SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83.SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83'"));
	if (StartAnimRef.Object) StartAnim = StartAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_RecallPlayerRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_RecallPlayer.MT_RecallPlayer'"));
	if (MT_RecallPlayerRef.Object) MT_RecallPlayer = MT_RecallPlayerRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ReadyToRandingRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_ReadyToRanding.MT_ReadyToRanding'"));
	if (MT_ReadyToRandingRef.Object) MT_ReadyToRanding = MT_ReadyToRandingRef.Object;

	CheckOnce = false;
}

void AHellpod::BeginPlay()
{
	Super::BeginPlay();

	HellpodMesh->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, -10000.0f));
}

void AHellpod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckToLanding();
}

void AHellpod::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UE_LOG(LogTemp, Log, TEXT("%s Damaged"), *OtherActor->GetName());
}

void AHellpod::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UE_LOG(LogTemp, Log, TEXT("%s Arrive"), *OtherActor->GetName());

	BoxCollider->SetGenerateOverlapEvents(false);

	HellpodMesh->SetCollisionProfileName(TEXT("BlockAll"));
	HellpodMesh->SetSimulatePhysics(false);

	FVector DropPoint = GetActorLocation();
	DropPoint.Z = SweepHitResult.ImpactPoint.Z;

	SetActorLocation(DropPoint);

	FTimerHandle DelaySummon;
	GetWorld()->GetTimerManager().SetTimer(DelaySummon, this, &AHellpod::SpawnAttachment, 1.0f, false);
}

void AHellpod::SpawnAttachment()
{
	HellpodMesh->BreakConstraint(FVector(0.0f, 0.0f, 35000.0f), FVector(0.0f, 0.0f, 35000.0f), TEXT("TOP_LID"));
	HellpodMesh->PlayAnimation(MT_RecallPlayer, false);
}

void AHellpod::CheckToLanding()
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

		HellpodMesh->PlayAnimation(MT_ReadyToRanding, false);
		HellpodMesh->SetLinearDamping(1.5f);
	}
}


