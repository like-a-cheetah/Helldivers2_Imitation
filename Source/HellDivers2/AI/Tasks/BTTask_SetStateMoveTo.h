// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_SetStateMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTTask_SetStateMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_SetStateMoveTo();

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};