// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/StratagemBall.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Stratagem/Stratagem.h"
#include "Stratagem/Hellpod.h"

AStratagemBall::AStratagemBall()
{
	TiggerCollision->DestroyComponent();

	SkelMeshComp->SetCollisionProfileName(TEXT("StratagemBall"));
	SkelMeshComp->SetNotifyRigidBodyCollision(true);

	NC_Laser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NC"));
	NC_Laser->SetRelativeLocation({ 0.0f, 0.0f, 7.f });
	NC_Laser->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_BlueRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/StratagemBall/NS_Blue.NS_Blue'"));
	if (NS_BlueRef.Object) NS_Blue = NS_BlueRef.Object;
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_RedRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/StratagemBall/NS_Red.NS_Red'"));
	if (NS_RedRef.Object) NS_Red = NS_RedRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_BallLoopRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Weapons/StratagemBall/BallBeacon.BallBeacon'"));
	if (SW_BallLoopRef.Object) SW_BallLoop = SW_BallLoopRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AS_LockupRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/PROPS/GAMEPLAY/Stratagem/SK_STRATAGEM_FIXED_Anim_0x36f434a85615ab80.SK_STRATAGEM_FIXED_Anim_0x36f434a85615ab80'"));
	if (AS_LockupRef.Object) AS_Lockup = AS_LockupRef.Object;

	SkelMeshComp->OnComponentHit.AddDynamic(this, &AStratagemBall::OnHit);
	//SkelMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AStratagemBall::DFS);
}

void AStratagemBall::BeginPlay()
{
	Super::BeginPlay();
} 

void AStratagemBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float DotProduct = FVector::DotProduct(Hit.Normal, FVector::UpVector);
	float AngleInRadians = FMath::Acos(DotProduct);
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

	if (AngleInDegrees > 30.0f)
		return;

	if (StratagemType == (uint8)EStratagemType::NotAttack) NC_Laser->SetAsset(NS_Blue);
	else NC_Laser->SetAsset(NS_Red);

	AbsoluteForwardVector = GetVelocity().Rotation();
	AbsoluteForwardVector.Pitch = 0.0f;

	FVector HitPoint = GetActorLocation();

	SkelMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	SkelMeshComp->SetNotifyRigidBodyCollision(false);
	SkelMeshComp->SetSimulatePhysics(false);
	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, false);
	AttachToActor(OtherActor, AttachRules);

	SetActorRotation({ 0.0f, 0.0f, 0.0f });

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, GetActorLocation(), GetActorUpVector() * -100.0f + GetActorLocation(), ECollisionChannel::ECC_WorldStatic))
	{
		SetActorLocation(HitResult.ImpactPoint);
	}
	else SetActorLocation(HitPoint);

	UGameplayStatics::PlaySoundAtLocation(this, SW_BallLoop, GetActorLocation());
	SkelMeshComp->PlayAnimation(AS_Lockup, false);

	SpawnStratagem();
}

void AStratagemBall::SpawnStratagem()
{
	FVector SpawnLoc = GetActorLocation();
	if (StratagemClass && StratagemClass->IsChildOf(AHellpod::StaticClass())) SpawnLoc.Z += 80000.0f;
	FRotator SpawnRot = FRotator::ZeroRotator;

	if (StratagemClass.Get())
	{
		AStratagem* Stratagem = GetWorld()->SpawnActor<AStratagem>(StratagemClass, SpawnLoc, AbsoluteForwardVector);

		Stratagem->OnDestoryBall.BindLambda([this]() {
			Destroy();
			});
	}
}