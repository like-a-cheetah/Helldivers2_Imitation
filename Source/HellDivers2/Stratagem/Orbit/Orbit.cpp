// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Orbit/Orbit.h"

#include "Math/UnrealMathUtility.h"

#include "Items/Rocket.h"

AOrbit::AOrbit()
{
	PrimaryActorTick.bCanEverTick = true;
	bStrike = false;
}

void AOrbit::BeginPlay()
{
	Super::BeginPlay();
}

void AOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector BallLoc = Ball->GetActorLocation();
	Goal = { BallLoc.X, BallLoc.Y, BallLoc.Z };

	if (!bStrike)
	{
		CalculateTiming(DeltaTime);
	}
	else
	{
		Strike(DeltaTime);
	}
}

void AOrbit::CalculateTiming(float DeltaTime)
{
	if (Times > 0)
	{
		TimeTerm -= DeltaTime;

		if (TimeTerm <= 0.0f)
		{
			bStrike = true;
			BombN = MaxBombN;

			TimeTerm = MaxTimeTerm;
			Times--;
		}
	}
	else
	{
		OnDestoryBall.Execute();
		Destroy();
	}
}

void AOrbit::Strike(float DeltaTime)
{
	if (BombN > 0)
	{
		FireTerm -= DeltaTime;

		if (FireTerm < 0) 
		{
			FVector SpawnLoc = FVector::ZeroVector;
			SpawnLoc.Z += 5000.0f;

			FVector2D Random2DPoint = FMath::RandPointInCircle(Radius);
			Goal = { Random2DPoint.X + Goal.X, Random2DPoint.Y + Goal.Y, Goal.Z };

			FVector NormalVector = Goal - SpawnLoc;
			NormalVector = NormalVector.GetSafeNormal();

			FRotator DirectRot = NormalVector.Rotation();

			AActor* SpawnedRocket = GetWorld()->SpawnActor<AActor>(RocketC, SpawnLoc, DirectRot);

			FireTerm = MaxFireTerm;
			BombN--;
		}
	}
	else bStrike = false;

}
