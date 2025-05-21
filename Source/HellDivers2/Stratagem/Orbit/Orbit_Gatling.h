// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Orbit/Orbit.h"
#include "Orbit_Gatling.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AOrbit_Gatling : public AOrbit
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetSpawnLoc() override;
	virtual void OrbitStartAction() override;
	virtual void OrbitEndAction() override;
};
