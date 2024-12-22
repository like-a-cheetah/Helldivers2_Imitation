// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Rocket.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Bullet.h"
#include "Characters/Player/PlayerCharacter.h"

ARocket::ARocket()
{
	MeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PROJECTS/HELLDIVERS_2/WEAPONS/STRATAGEM/GR-8_RECOILESS/Rocket/StaticMesh.StaticMesh'"));
	if(MeshRef.Object)
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	};

	SphereComp->OnComponentBeginOverlap.RemoveAll(this);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARocket::OnOverlapBegin_Rocket);

	Splash = CreateDefaultSubobject<USphereComponent>(TEXT("Splash"));
	Splash->SetCollisionProfileName(TEXT("Splash"));
	Splash->SetEnableGravity(false);
	Splash->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
	Splash->OnComponentBeginOverlap.AddDynamic(this, &ARocket::OnOverlapBegin_Splash);
	Splash->SetupAttachment(MeshComp);

	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;

	BoomVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BoomVFX"));
	BoomVFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	{
		BoomVFX->SetTemplate(ParticleRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/NP_Rocket.NP_Rocket'"));
	if (TrailRef.Object)
	{
		Trail->SetAsset(TrailRef.Object);
	}

	Trail->SetupAttachment(RootComponent);
	Trail->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules AttachmentTransformRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Trail->AttachToComponent(MeshComp, AttachmentTransformRule, TEXT("attach_trail"));
	BoomVFX->DeactivateSystem();

	Splash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARocket::OnOverlapBegin_Rocket(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProjectileMovementComp->StopMovementImmediately();

	MeshComp->SetHiddenInGame(true);
	Trail->SetHiddenInGame(true);
	BoomVFX->ActivateSystem();

	SphereComp->OnComponentBeginOverlap.RemoveAll(this);

	Splash->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Splash->SetSimulatePhysics(false);
			Splash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}, 0.1f, false);
}

void ARocket::OnOverlapBegin_Splash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Splash->SetSimulatePhysics(true);

	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		//Player->BeAttacked(100.0f);

		FVector Force = OtherActor->GetActorLocation() - GetActorLocation();
		Force = Force.GetSafeNormal() * 2000.0f;

		Cast<ACharacter>(Player)->LaunchCharacter(Force, true, true);

		return;
	}
}
