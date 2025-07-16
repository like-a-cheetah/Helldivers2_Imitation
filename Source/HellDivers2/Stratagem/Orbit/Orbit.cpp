// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Orbit/Orbit.h"

#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"

#include "Items/Rocket.h"

AOrbit::AOrbit()
{
	PrimaryActorTick.bCanEverTick = true;
	bStrike = false;
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AShip> It(GetWorld()); It; ++It)
	{
		Ship = *It;
		break;
	}

	SetSpawnLoc();
}

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FQuat RotationQuat = FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(90.0f));  
	//
	//FVector CirclePos = GetActorLocation();
	//CirclePos.Z += 20.f;
	//FTransform Transform(RotationQuat, CirclePos);
	//
	//FMatrix TransformMatrix = Transform.ToMatrixWithScale();
	//
	//DrawDebugCircle(
	//	GetWorld(),        // 월드 객체
	//	TransformMatrix,   // 변환 행렬 (회전 및 위치)
	//	Radius,            // 반지름
	//	32,          // 세그먼트 수
	//	FColor::Red,       // 색상
	//	false,             // bPersistentLines
	//	0.1f,              // LifeTime (0.1초 동안 유지)
	//	0,                 // DepthPriority
	//	20.f,              // 선 두께
	//	false               // bDrawAxis
	//);

	if (!bStrike)
	{
		CalculateTiming(DeltaTime);
	}
	else
	{
		ElapsedTime += DeltaTime;
		Strike(DeltaTime);
	}
}

void AOrbit::CalculateTiming(float DeltaTime)
{
	if (Times > 0)
	{
		TimeTerm -= DeltaTime;

		if (TimeTerm <= 0.0f)
			OrbitStartAction();
	}
	else
	{
		OrbitEndAction();
	}
}

void AOrbit::Strike(float DeltaTime)
{
	if (BombN > 0)
	{
		FireTerm -= DeltaTime;

		if (FireTerm < 0) 
		{
			SetStrikePoint();

			FireTerm = MaxFireTerm;
			BombN--;
		}
	}
	else bStrike = false;
}

void AOrbit::SetStrikePoint()
{
	FVector ActorLoc = GetActorLocation();
	FVector2D Random2DPoint = FMath::RandPointInCircle(Radius);
	Goal = { Random2DPoint.X + ActorLoc.X, Random2DPoint.Y + ActorLoc.Y, ActorLoc.Z };

	FVector NormalVector = Goal - SpawnLoc;
	NormalVector = NormalVector.GetSafeNormal();

	FRotator DirectRot = NormalVector.Rotation();

	AActor* SpawnedRocket = GetWorld()->SpawnActor<AActor>(RocketC, SpawnLoc, DirectRot);
}

void AOrbit::SetSpawnLoc()
{
	SpawnLoc = Ship->GetSocketLocation(MuzzleSocketNames[SocketN]);
	SocketN++;
	if (SocketN >= MuzzleSocketNames.Num()) SocketN = 0;
}

void AOrbit::OrbitStartAction()
{
	bStrike = true;
	BombN = MaxBombN;

	TimeTerm = MaxTimeTerm;
	Times--;
}

void AOrbit::OrbitEndAction()
{
	OnDestoryBall.ExecuteIfBound();
	Destroy();
}
