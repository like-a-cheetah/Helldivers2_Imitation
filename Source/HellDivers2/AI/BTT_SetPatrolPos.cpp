// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_SetPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/EnemyAIInterface.h"
#include "Characters/Components/EnemyKey.h"

EBTNodeResult::Type UBTT_SetPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlPawn->GetWorld());
	if(NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IEnemyAIInterface* PawnAI = Cast<IEnemyAIInterface>(ControlPawn);
	if (PawnAI == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = PawnAI->GetPatrolRadius();
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	//if (NavSystem->GetRandomPointInNavigableRadius(ControlPawn->GetActorLocation(), PatrolRadius, NextPatrolPos))
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, { 5250.000000, 37.000000, 176.456177 });
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return Result;
}
