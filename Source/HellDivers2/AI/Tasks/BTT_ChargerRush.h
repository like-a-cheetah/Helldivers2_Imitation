// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargerRush.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTT_ChargerRush : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ChargerRush();

private:
	TObjectPtr<class ACharger> Charger;
	float Duration;
	uint8 bStartedRush : 1;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//void OnRushFinished(UBehaviorTreeComponent* OwnerComp);
};
