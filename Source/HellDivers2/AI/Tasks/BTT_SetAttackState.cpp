// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SetAttackState.h"

#include "AIController.h"

#include "Interface/AnimMovementInterface.h"

EBTNodeResult::Type UBTT_SetAttackState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAnimMovementInterface* Enemy = Cast<IAnimMovementInterface>(ControlPawn);
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->SetAttack();
	Enemy->OnAttackEnded.BindLambda([this, &OwnerComp]() {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	return EBTNodeResult::InProgress;
}
