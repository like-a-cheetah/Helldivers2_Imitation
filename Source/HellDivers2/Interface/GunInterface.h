// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IGunInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UFUNCTION(BlueprintCallable, Category = Gun)
	virtual void Shot() = 0;

	//UFUNCTION(BlueprintCallable, Category = Gun)
	virtual void Reload() = 0;

	virtual bool IsBurst() = 0;

	virtual FVector GetImpactPoint() = 0;

	virtual UAnimMontage* GetReloadMontage() = 0;

	virtual void PlayReloadMontage() = 0;

	virtual bool IsCompleteReload() = 0;

	virtual void StopShotSound() = 0;
};
