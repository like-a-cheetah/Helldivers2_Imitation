// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_BB_SetRandVar.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTS_BB_SetRandVar : public UBTService_BlackboardBase
{
	GENERATED_BODY()

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Range;
};
