// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Grenade.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

#include "Characters/Player/PlayerCharacter.h"

AGrenade::AGrenade()
{
	VFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	if (ParticleRef.Object)
	{
		VFX->SetTemplate(ParticleRef.Object);
	}
	VFX->SetSimulatePhysics(false);
	VFX->SetupAttachment(RootComponent);

	Splash = CreateDefaultSubobject<USphereComponent>(TEXT("Splash"));
	Splash->SetCollisionProfileName(TEXT("Splash"));
	Splash->SetEnableGravity(false);
	Splash->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
	Splash->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnExplosionRangeBeginOverlap);
	Splash->SetupAttachment(RootComponent);

	FuseTime = 3.0f;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	VFX->DeactivateSystem();

	Splash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGrenade::OnExplosionRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Splash->SetSimulatePhysics(true);

	FVector Force = OtherActor->GetActorLocation() - GetActorLocation();
	Force = Force.GetSafeNormal() * 3000.0f;
}

void AGrenade::Bomb()
{
	VFX->Activate(true);

	SkelMeshComp->SetHiddenInGame(true);

	Splash->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Splash->SetSimulatePhysics(true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Splash->SetSimulatePhysics(false);
			Splash->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}, 0.1f, false);
}

void AGrenade::PullingPin()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGrenade::Bomb, FuseTime, false);
}
