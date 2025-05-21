// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_BasicAttack.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Monsters/BileTitan.h"

EBTNodeResult::Type UBTT_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABileTitan* Titan = Cast<ABileTitan>(ControlPawn);
	if (Titan == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FOnMontageEnded MT_EndDelegate;
	MT_EndDelegate.BindLambda([this, &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	Titan->SetStop();
	Titan->StartBasicAttack(MT_EndDelegate);

	return EBTNodeResult::InProgress;
}
