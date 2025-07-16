// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IEnemyAIInterface
{
	GENERATED_BODY()

protected:
	float PatrolRadius;
	TObjectPtr<AActor> Target;

public:
	virtual float GetBodyRadius() = 0;
	virtual float GetBodyHalfHeight() = 0;
	virtual float GetPatrolRadius() = 0;
	virtual void SetTarget(AActor* InTarget) = 0;
	virtual AActor* GetTarget() = 0;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	//float GetPatrolRadius() const;

	//// Setter for target
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	//void SetTarget(AActor* InTarget);
};
