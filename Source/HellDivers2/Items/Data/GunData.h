// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GunData.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UGunData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGunData();

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint8 bBurst : 1;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float RecoilVal;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float FireRate;

	UPROPERTY(EditAnywhere, Category = Weapon)
	uint32 MaxRounds;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class ABullet> Bullet;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UAnimMontage> MT_PlayerReload;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UAnimationAsset> MT_GunReload;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float MaxPitch;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float MinPitch;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float MaxYaw;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float MinYaw;
};
