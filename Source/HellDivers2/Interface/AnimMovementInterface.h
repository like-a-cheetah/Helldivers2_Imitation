// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

DECLARE_DELEGATE(FOnAttackEnded);

class HELLDIVERS2_API IAnimMovementInterface
{
	GENERATED_BODY()

public:
	virtual void SetWalk() = 0;
	virtual void SetTrace() = 0;
	virtual void SetStop() = 0;
	virtual void SetAttack() = 0;
	virtual void SetRotate(float Angle) = 0;

	FOnAttackEnded OnAttackEnded;
};
