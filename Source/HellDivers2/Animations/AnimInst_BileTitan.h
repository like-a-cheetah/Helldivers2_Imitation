// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/AnimInst_Enemy.h"

#include "Characters/Monsters/BileTitan.h"
#include "AnimInst_BileTitan.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UAnimInst_BileTitan : public UAnimInst_Enemy
{
	GENERATED_BODY()

public:
	UAnimInst_BileTitan();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
