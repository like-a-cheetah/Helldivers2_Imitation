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

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> SphereComp;

	TObjectPtr<class AEnemy> Target;

	TArray<AActor*> OverlappedActor;

	float TempTargetDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraComponent> NC_Laser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> PS_Spark;

	UPROPERTY()
	TSubclassOf<class AFire> Fire;

	float MaxFireSpawnTime;
	float FireSpawnTime;

	FVector SpawnLoc;
	FVector LaserEndPoint;
	FVector Direction;
	FQuat QuatDirectRotation;
	FVector EndPoint;
};
