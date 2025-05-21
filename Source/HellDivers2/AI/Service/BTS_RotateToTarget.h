// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTS_RotateToTarget : public UBTService
{
	GENERATED_BODY()

//public:
//	UBTS_RotateToTarget();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRelativeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed;
};
