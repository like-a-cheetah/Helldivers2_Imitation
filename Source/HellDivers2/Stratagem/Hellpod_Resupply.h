// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Hellpod_Supply.h"
#include "Hellpod_Resupply.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHellpod_Resupply : public AHellpod_Supply
{
	GENERATED_BODY()
	
public:
	AHellpod_Resupply();

	virtual void BeginPlay() override;

	// AHellpod_Supply을(를) 통해 상속됨
	void SpawnSupplyItems() override;
};
