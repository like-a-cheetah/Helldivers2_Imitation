// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AcidBall.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Niagara/Classes/NiagaraSystem.h"

AAcidBall::AAcidBall()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComponent"));
	RootComponent = SphereComp;
	SphereComp->InitSphereRadius(60.0f);
	SphereComp->SetCollisionProfileName(FName(TEXT("Trigger")));
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAcidBall::OnAcidBeginOverlap);

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

}

void AAcidBall::OnAcidBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		//UE_LOG(LogTemp, Log, TEXT("%s Damaged"), *OtherActor->GetName());
	}
}
