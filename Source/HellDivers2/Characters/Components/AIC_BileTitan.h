// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/AIController_Enemy.h"
#include "AIC_BileTitan.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AAIC_BileTitan : public AAIController_Enemy
{
	GENERATED_BODY()
	
public:
	AAIC_BileTitan();

	void Tick(float DeltaTime) override;

private:
	void DecreaseAcidCoolTime(float DeltaTime);

};
