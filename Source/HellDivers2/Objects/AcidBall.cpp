// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AcidBall.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "Characters/Components/CharacterStatComponent.h"

AAcidBall::AAcidBall()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComponent"));
	RootComponent = SphereComp;
	SphereComp->InitSphereRadius(60.0f);
	SphereComp->SetCollisionProfileName(FName(TEXT("EnemyOverlapAttack")));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAcidBall::OnAcidBeginOverlap);

	SplashColliderComp = CreateDefaultSubobject<USphereComponent>(TEXT("SplashColliderComponent"));
	SplashColliderComp->SetupAttachment(RootComponent);
	SplashColliderComp->InitSphereRadius(200.0f);
	SplashColliderComp->SetCollisionProfileName(FName(TEXT("Splash")));
	SplashColliderComp->OnComponentBeginOverlap.AddDynamic(this, &AAcidBall::OnSplashBeginOverlap);
	SplashColliderComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	Particle->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ParticleRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/VFX/Trail_10_09.Trail_10_09'"));
	if (ParticleRef.Succeeded())
	{
		Particle->SetAsset(ParticleRef.Object);
	}

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 4000.0f;
	ProjectileMovementComp->MaxSpeed = 4000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BoomParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A1.P_Explosion_Big_A1'"));
	if (BoomParticleRef.Object) BoomParticle = BoomParticleRef.Object;
}

void AAcidBall::OnAcidBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner() && OtherActor != this)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BoomParticle,
			GetActorLocation(),
			FRotator::ZeroRotator,
			FVector(1, 1, 1)
		);

		SplashColliderComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		Destroy();
	}
}

void AAcidBall::OnSplashBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag(TEXT("Player")))
	{
		UCharacterStatComponent* CharacterStat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
		CharacterStat->ApplyDamage(30.0f);

		FVector Direct = OtherActor->GetActorLocation() - GetActorLocation();
		Direct.Normalize();
		Direct *= 300.0f;
		Direct.Z = 300.0f;

		Cast<ACharacter>(OtherActor)->LaunchCharacter(Direct, false, false);
	}
}
