// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Stratagem.h"
#include "Orbit.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AOrbit : public AStratagem
{
	GENERATED_BODY()
	
public:
	AOrbit();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	TObjectPtr<AShip> Ship;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	TArray<FName> MuzzleSocketNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	TSubclassOf<class ABullet> RocketC;

	UPROPERTY();
	FVector Goal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	int32 Times;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	int32 MaxBombN;
	int32 BombN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	float MaxTimeTerm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	float TimeTerm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	float MaxFireTerm;
	float FireTerm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Data")
	uint8 bStrike : 1;

	void CalculateTiming(float DeltaTime);
	virtual void Strike(float DeltaTime);
	virtual void SetStrikePoint();
	virtual void SetSpawnLoc();
	virtual void OrbitStartAction();
	virtual void OrbitEndAction();

	float ElapsedTime;

	int SocketN;
	
	FVector SpawnLoc;
};
