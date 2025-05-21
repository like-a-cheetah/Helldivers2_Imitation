// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Orbit/Orbit.h"

#include "Engine/OverlapResult.h"

#include "Orbit_Laser.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AOrbit_Laser : public AOrbit
{
	GENERATED_BODY()

public:
	AOrbit_Laser();
	virtual void BeginPlay() override;

protected:
	virtual void Strike(float DeltaTime);
	virtual void SetStrikePoint();
	virtual void SetSpawnLoc() override;

	bool LaserOverlapAccess(float DeltaTime);

	bool SetLaser();

	//void SpreadFire();
	void SpreadFire(AActor* Actor = nullptr);

private:
	TObjectPtr<class AEnemy> Target;
	float TempTargetDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraComponent> NC_Laser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> PS_Spark;

	TSubclassOf<class AFire> Fire;

	float MaxFireSpawnTime;
	float FireSpawnTime;

	FVector SpawnLoc;
	FVector LaserEndPoint;
	FVector Direction;
	FQuat QuatDirectRotation;
	FVector EndPoint;
};
