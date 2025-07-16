// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Rocket.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"

#include "Bullet.h"
#include "Characters/Components/CharacterStatComponent.h"

ARocket::ARocket()
{
	ProjectileMovementComp->InitialSpeed = 40000.0f;
	ProjectileMovementComp->MaxSpeed = 40000.0f;

	MeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PROJECTS/HELLDIVERS_2/WEAPONS/STRATAGEM/GR-8_RECOILESS/Rocket/StaticMesh.StaticMesh'"));
	if(MeshRef.Object)
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	};

	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileMovementComp->InitialSpeed;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	if (ParticleRef.Object) HitVFX = ParticleRef.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/NP_Rocket.NP_Rocket'"));
	if (TrailRef.Object) Trail->SetAsset(TrailRef.Object);

	Trail->SetupAttachment(RootComponent);
	Trail->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	Damage = 100.0f;
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules AttachmentTransformRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Trail->AttachToComponent(MeshComp, AttachmentTransformRule, TEXT("attach_trail"));
}

void ARocket::HitPostProcess(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	Super::HitPostProcess(OtherActor, OtherComp, Hit);

	USphereComponent* SplashCollision = NewObject<USphereComponent>(OtherComp, TEXT("fds"));
	//SplashCollision->AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
	SplashCollision->SetWorldLocation(Hit.ImpactPoint);

	SplashCollision->InitSphereRadius(SplashRadius);
	SplashCollision->SetCollisionProfileName(TEXT("Splash"));
	SplashCollision->SetGenerateOverlapEvents(true);
	SplashCollision->OnComponentBeginOverlap.AddDynamic(this, &ARocket::OnOverlapBegin_Splash);

	//SplashCollision->SetVisibility(true);
	//SplashCollision->SetHiddenInGame(false);

	SplashCollision->RegisterComponent();
	SplashCollision->UpdateOverlaps();
}

void ARocket::OnOverlapBegin_Splash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCharacterStatComponent* CharacterStat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
	if (CharacterStat)
	{
		CharacterStat->ApplyDamage(Damage);

		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			FVector Force = OtherActor->GetActorLocation() - GetActorLocation();
			Force = Force.GetSafeNormal() * 2000.0f;

			Character->LaunchCharacter(Force, true, true);
		}
	}
}
