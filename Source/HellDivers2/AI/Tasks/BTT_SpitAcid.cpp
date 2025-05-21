// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SpitAcid.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Monsters/BileTitan.h"
#include "Characters/Components/EnemyKey.h"
#include "Characters/Components/AcidComponent.h"

EBTNodeResult::Type UBTT_SpitAcid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target || !Titan->GetAcidComp())
	{
		return EBTNodeResult::Failed;
	}

	FOnMontageEnded MT_EndDelegate;
	MT_EndDelegate.BindLambda([this, &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	Titan->StartSpitAcid(MT_EndDelegate);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_ACID_COOLTIME, 15.0f);

	return EBTNodeResult::InProgress;
}
