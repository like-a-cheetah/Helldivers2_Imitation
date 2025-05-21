// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetRandVar.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTT_SetRandVar : public UBTTaskNode
{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Range;
};
