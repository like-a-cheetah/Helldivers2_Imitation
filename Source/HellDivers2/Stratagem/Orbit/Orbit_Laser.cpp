// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Orbit/Orbit_Laser.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"

#include "Characters/Monsters/Enemy.h"
#include "Objects/Fire.h"

AOrbit_Laser::AOrbit_Laser()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    NC_Laser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NC_Laser"));
    NC_Laser->SetCollisionProfileName(TEXT("BlockStaticAndOverlapChar"));
    NC_Laser->SetGenerateOverlapEvents(true);
    //NC_Laser->Overlap
    if (NC_Laser) {
        static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_BlueRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/HellDivers2/Weapons/StratagemBall/NS_Laser1.NS_Laser1'"));
        if (NS_BlueRef.Object) NC_Laser->SetAsset(NS_BlueRef.Object);
    }

    PS_Spark = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_Spark"));
    if(PS_Spark) {
        static ConstructorHelpers::FObjectFinder<UParticleSystem> SparkParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Brick.P_Brick'"));
        if (SparkParticleRef.Object) PS_Spark->SetTemplate(SparkParticleRef.Object);
    }

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OnlyActorOverlap"));
    SphereComp->SetupAttachment(RootComponent);
    SphereComp->SetCollisionProfileName(TEXT("Splash"));

    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AOrbit_Laser::OnOverlapBegin);
    SphereComp->OnComponentEndOverlap.AddDynamic(this, &AOrbit_Laser::OnOverlapEnd);

    MaxFireSpawnTime = 0.2f;
}

void AOrbit_Laser::BeginPlay()
{
    Super::BeginPlay();

    SphereComp->SetSphereRadius(Radius);

    NC_Laser->SetWorldLocation(SpawnLoc);
    NC_Laser->SetNiagaraVariableVec3(TEXT("BeamEndLocation"), SpawnLoc);

    Fire = AFire::StaticClass();
}

void AOrbit_Laser::Strike(float DeltaTime)
{
    if (Duration > 0.0f)
    {
        FVector TargetPos = GetActorLocation();

        if (!Target)
        {
            SetStrikePoint();
        }
        else
        {
            UCharacterStatComponent* EnemyStat = Target->FindComponentByClass<UCharacterStatComponent>();
            if (EnemyStat)
            {
                if (EnemyStat->GetCurHp() <= 0.0f)
                    SetStrikePoint();
            }
        }

        if (Target) TargetPos = Target->GetActorLocation();

        float DeltaMove = 1000.f * DeltaTime;
        FVector NextPos;
        if (FVector::Distance(GetActorLocation(), TargetPos) <= DeltaMove)
            NextPos = TargetPos;
        else
            NextPos = GetActorLocation() + (TargetPos - GetActorLocation()).GetSafeNormal() * DeltaMove;
        SetActorLocation(NextPos);

        bool bHitGround = SetLaser();
        bool bHitEnemy = LaserOverlapAccess(DeltaTime);
        if (bHitGround && !bHitEnemy)
        {
            if (FireSpawnTime <= 0.0f)
                SpreadFire();
        }

        FireSpawnTime -= DeltaTime;
        Duration -= DeltaTime;
    }
    else
    {
        OnDestoryBall.Execute();
        Destroy();
    }
}

void AOrbit_Laser::SetStrikePoint()
{
    FVector StratagemLoc = GetActorLocation();

    if (OverlappedActor.Num() > 0)
    {
        AEnemy* BiggestEnemy = nullptr;
        for (AActor* Actor : OverlappedActor)
        {
            AEnemy* Enemy = Cast<AEnemy>(Actor);

            UCharacterStatComponent* Stat = Enemy->FindComponentByClass<UCharacterStatComponent>();
            if (Stat && Stat->GetCurHp() > 0.0f)
            {
                if (BiggestEnemy)
                {
                    if (Enemy->GetBodyRadius() > BiggestEnemy->GetBodyRadius())
                    {
                        BiggestEnemy = Enemy;
                        TempTargetDist = FVector::Distance(StratagemLoc, BiggestEnemy->GetActorLocation());
                    }
                    else if (Enemy->GetBodyRadius() == BiggestEnemy->GetBodyRadius())
                    {
                        float NewDist = FVector::Distance(StratagemLoc, Enemy->GetActorLocation());
                        if (NewDist < TempTargetDist)
                        {
                            BiggestEnemy = Enemy;
                            TempTargetDist = NewDist;
                        }
                    }
                }
                else
                {
                    BiggestEnemy = Enemy;
                    TempTargetDist = FVector::Distance(StratagemLoc, BiggestEnemy->GetActorLocation());
                }
            }
        }

        if (BiggestEnemy)
        {
            Target = BiggestEnemy;
        }
    }
}

void AOrbit_Laser::SetSpawnLoc()
{
    SpawnLoc = Ship->GetSocketLocation(MuzzleSocketNames[0]);
}

bool AOrbit_Laser::LaserOverlapAccess(float DeltaTime)
{
    float CapsuleHeight = FVector::Distance(SpawnLoc, EndPoint) / 2.0f;

    FVector AdjustedSpawnLoc = SpawnLoc + Direction * CapsuleHeight;

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->OverlapMultiByProfile(
        OverlapResults,
        AdjustedSpawnLoc,
        QuatDirectRotation,
        TEXT("BlockStaticAndOverlapChar"),
        FCollisionShape::MakeCapsule(1.f, CapsuleHeight),
        CollisionParams
    );
    //DrawDebugCapsule(GetWorld(), AdjustedSpawnLoc, CapsuleHeight, 100.0f, QuatDirectRotation, FColor::Yellow, false, .1f, 0U, 5.0f);

    bool bHitEnemy = false;
    for (const FOverlapResult& Result : OverlapResults)
    {
        //UE_LOG(LogTemp, Log, TEXT("%s"), *Result.GetActor()->GetName());

        if (Result.GetActor())
        {
            UCharacterStatComponent* Stat = Result.GetActor()->FindComponentByClass<UCharacterStatComponent>();
            if (Stat)
            {
                Stat->ApplyDamage(70.f * DeltaTime);
                bHitEnemy = true;

                if (Result.GetActor() == Target)
                    Target = nullptr;

                if (FireSpawnTime <= 0.0f)
                    SpreadFire(Result.GetActor());
            }
        }
    }

    return bHitEnemy;
}

bool AOrbit_Laser::SetLaser()
{
    Direction = GetActorLocation() - SpawnLoc;
    Direction.Normalize();
    QuatDirectRotation = FQuat::FindBetweenNormals(FVector::UpVector, Direction);

    TArray<FHitResult> HitResults;
    FVector EndLoc = SpawnLoc + Direction * 100000.0f;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    //bool bHit = GetWorld()->LineTraceMultiByChannel(HitResults, SpawnLoc, EndLoc, ECC_WorldStatic);
    bool bHit = GetWorld()->LineTraceMultiByProfile(HitResults, SpawnLoc, EndLoc, TEXT("BlockStaticAndOverlapChar"), Params);
    if (bHit)
    {
        FHitResult ClosestHit;
        float MinDistance = FLT_MAX;
        for (const FHitResult& Hit : HitResults)
        {
            float Distance = FVector::Dist(SpawnLoc, Hit.ImpactPoint);
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestHit = Hit;
            }
        }

        EndPoint = ClosestHit.ImpactPoint;
    }
    else EndPoint = EndLoc;

    NC_Laser->SetNiagaraVariableVec3(TEXT("BeamEndLocation"), EndPoint);

    FQuat SparkRot = FQuat::FindBetweenNormals(FVector::UpVector, -Direction);
    PS_Spark->SetWorldLocation(EndPoint);
    PS_Spark->SetWorldRotation(SparkRot);

    return bHit;
}

void AOrbit_Laser::SpreadFire(AActor* Actor)
{
    FTransform FireArea(FRotator::ZeroRotator, EndPoint);
    AFire* SpawnedFire;
    if (Actor)
    {
        SpawnedFire = GetWorld()->SpawnActor<AFire>(Fire, FireArea);
        SpawnedFire->AttachToActor(Actor, FAttachmentTransformRules::KeepWorldTransform);
    }
    else SpawnedFire = GetWorld()->SpawnActor<AFire>(Fire, FireArea);

    if (SpawnedFire) FireSpawnTime = MaxFireSpawnTime;
}

void AOrbit_Laser::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<AEnemy>(OtherActor))
        OverlappedActor.Add(OtherActor);
}

void AOrbit_Laser::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(OverlappedActor.Find(OtherActor))
        OverlappedActor.Remove(OtherActor);
}
