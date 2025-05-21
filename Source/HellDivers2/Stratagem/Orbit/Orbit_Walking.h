// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Orbit/Orbit.h"
#include "Orbit_Walking.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AOrbit_Walking : public AOrbit
{
	GENERATED_BODY()
	
protected:
	virtual void SetStrikePoint() override;
};
