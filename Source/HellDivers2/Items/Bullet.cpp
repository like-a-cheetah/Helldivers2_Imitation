// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Monsters/Enemy.h"
#include "Characters/Components/CharacterStatComponent.h"

FOnHitEnemy ABullet::OnHitEnemy;

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionProfileName(TEXT("Bullet"));
	MeshComp->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	MeshComp->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PROJECTS/HELLDIVERS_2/WEAPONS_v2/LIBERATOR/StaticMesh.StaticMesh'"));
	if (MeshRef.Object)
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	}

	RootComponent = MeshComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 20000.0f;
	ProjectileMovementComp->MaxSpeed = 20000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->Velocity = GetActorRightVector() * ProjectileMovementComp->InitialSpeed;

	Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TrailRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/NP_bullet_clone.NP_bullet_clone'"));
	if (TrailRef.Object)
	{
		Trail->SetAsset(TrailRef.Object);
	}
	Trail->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Trail->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default2.P_Default2'"));
	if(ParticleRef.Object) HitVFX = ParticleRef.Object;

	Damage = 10.0f;

	HitVFXScale = 1.0f;

	SetLifeSpan(10.0f);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitPostProcess(OtherActor, OtherComp, Hit);

	Destroy();
}

void ABullet::HitPostProcess(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Hit : %s's %s' Attacked"), *OtherActor->GetName(), *OtherComp->GetName());

	UCharacterStatComponent* EnemyStat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
	if (EnemyStat)
	{
		EnemyStat->ApplyDamage(Damage);
		if(OnHitEnemy.IsBound()) OnHitEnemy.Execute();

		if(GetOwner()) UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, GetOwner(), UDamageType::StaticClass());
	}

	FTransform ImpactTransform;
	ImpactTransform.SetLocation(Hit.ImpactPoint);
	FRotator fds = GetActorRotation();
	fds.Roll -= 90.0f;
	ImpactTransform.SetRotation(fds.Quaternion());
	ImpactTransform.SetScale3D(FVector(HitVFXScale, HitVFXScale, HitVFXScale));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFX, ImpactTransform);
}
	

