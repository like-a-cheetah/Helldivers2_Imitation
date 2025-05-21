// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/AIController_Enemy.h"
#include "AIC_Charger.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AAIC_Charger : public AAIController_Enemy
{
	GENERATED_BODY()
	
public:
	AAIC_Charger();

	void Tick(float DeltaTime) override;
};
