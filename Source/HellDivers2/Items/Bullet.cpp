// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PROJECTS/HELLDIVERS_2/WEAPONS_v2/LIBERATOR/StaticMesh.StaticMesh'"));
	if (MeshRef.Object)
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	}

	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionProfileName(TEXT("Bullet"));
	SphereComp->SetEnableGravity(false);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletBeginOverlap);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 4000.0f;
	ProjectileMovementComp->MaxSpeed = 4000.0f;
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
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
}
	

