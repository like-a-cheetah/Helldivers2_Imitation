// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

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

	SetRootComponent(HellpodMesh);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(HellpodMesh);
	BoxCollider->SetCollisionProfileName(TEXT("OverlapOnlyStatic"));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHellpod::OnBoxOverlapBegin);

	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireRef(TEXT("/Script/Engine.ParticleSystem'/Game/PROJECTS/HELLDIVERS_2/VFX_PROJECT/RoboRecall/P_RocketTrail/Particles/P_Trail_Rocket_SIMPLE_ALT_HD3.P_Trail_Rocket_SIMPLE_ALT_HD3'"));
	if (FireRef.Succeeded()) FireParticle->SetTemplate(FireRef.Object);
	FireParticle->SetupAttachment(RootComponent);
	FireParticle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FireParticle->SetRelativeLocation(FVector(0, 0, -468.333591f));
	FireParticle->SetRelativeRotation(FRotator(90, 0, 0));
	FireParticle->SetRelativeScale3D(FVector(9, 16, 16));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bAutoActivate = false;        
	AudioComp->bAllowSpatialization = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/SK_HELLPOD_PLAYER.SK_HELLPOD_PLAYER'"));
	if (SkelMeshRef.Succeeded()) HellpodMesh->SetSkeletalMeshAsset(SkelMeshRef.Object);

	static ConstructorHelpers::FObjectFinder<UAnimSequence> StartAnimRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/HELLPOD_VARIANTS/HELLPOD_PLAYER/Animations/SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83.SK_HELLPOD_PLAYER_Anim_0xf1c7319db45bea83'"));
	if (StartAnimRef.Object) StartAnim = StartAnimRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_RecallPlayerRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_RecallPlayer.MT_RecallPlayer'"));
	if (MT_RecallPlayerRef.Object) MT_RecallPlayer = MT_RecallPlayerRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ReadyToRandingRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Stratagem/HellPodPlayer/MT_ReadyToRanding.MT_ReadyToRanding'"));
	if (MT_ReadyToRandingRef.Object) MT_ReadyToRanding = MT_ReadyToRandingRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_FallingRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Stratagem/Source/hellpod_falling_screech.hellpod_falling_screech'"));
	if (SW_FallingRef.Object) SW_Falling = SW_FallingRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_LandingRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Stratagem/Source/hellpod_land_hit.hellpod_land_hit'"));
	if (SW_LandingRef.Object) SW_Landing = SW_LandingRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_HatchOpenRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Stratagem/Source/Open_Hellpod_Hatch_0x3cec0b1c500ab97.Open_Hellpod_Hatch_0x3cec0b1c500ab97'"));
	if (SW_HatchOpenRef.Object) SW_HatchOpen = SW_HatchOpenRef.Object;

	bDecisionLandingPoint = false;

	HellpodMesh->BodyInstance.bLockXRotation = true;
	HellpodMesh->BodyInstance.bLockYRotation = true;
	HellpodMesh->BodyInstance.bLockZRotation = true;
}

void AHellpod::BeginPlay()
{
	Super::BeginPlay();

	AudioComp->SetSound(SW_Falling);
	AudioComp->Play();

	HellpodMesh->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, -10000.0f));
}

void AHellpod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bDecisionLandingPoint) CheckToLanding();
}

void AHellpod::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
}

void AHellpod::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor == this) return;

	BoxCollider->SetGenerateOverlapEvents(false);

	if (OnDestoryBall.IsBound()) OnDestoryBall.Execute();

	HellpodMesh->SetSimulatePhysics(false);
	HellpodMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HellpodMesh->SetCollisionProfileName(TEXT("BlockAll"));
	HellpodMesh->SetCollisionObjectType(ECC_WorldDynamic);
	HellpodMesh->SetCollisionResponseToAllChannels(ECR_Block);
	HellpodMesh->SetGenerateOverlapEvents(false);
	HellpodMesh->SetAllBodiesSimulatePhysics(false);
	HellpodMesh->SetAllBodiesPhysicsBlendWeight(0.0f);

	FVector DropPoint = GetActorLocation();

	SetActorLocation(DropPoint);

	AudioComp->Stop();
	AudioComp->SetSound(SW_Landing);
	AudioComp->Play();

	FTimerHandle DelaySummon;
	GetWorld()->GetTimerManager().SetTimer(DelaySummon, this, &AHellpod::SpawnAttachMachine, 1.0f, false);
}

void AHellpod::SpawnAttachMachine()
{
	HellpodMesh->BreakConstraint(FVector(0.0f, 0.0f, 35000.0f), FVector(0.0f, 0.0f, 35000.0f), TEXT("TOP_LID"));

	AudioComp->Stop();
	AudioComp->SetSound(SW_HatchOpen);
	AudioComp->Play();
}

void AHellpod::CheckToLanding()
{
	FVector StartPos = GetActorLocation();
	StartPos.Z -= 500.0f;
	FVector EndPos = StartPos;
	EndPos.Z -= 13000.0f;
	float Radius = 50.0f;
	FHitResult OutHit;

	TArray<AActor*> Temp;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartPos,
		EndPos,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel7),
		false,
		Temp,
		EDrawDebugTrace::None,
		OutHit,
		true
	);

	if (bHit && !bDecisionLandingPoint) {
		bDecisionLandingPoint = true;

		HellpodMesh->PlayAnimation(MT_ReadyToRanding, false);
		HellpodMesh->SetLinearDamping(1.f);

		FireParticle->SetActive(false);
	}
}


