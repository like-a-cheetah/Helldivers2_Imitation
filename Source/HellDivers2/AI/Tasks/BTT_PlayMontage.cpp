// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_PlayMontage.h"

#include "AIController.h"

#include "Characters/Monsters/Enemy.h"

EBTNodeResult::Type UBTT_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	AEnemy* Enemy = Cast<AEnemy>(OwnerPawn);
	if (!Enemy || !Montage)
	{
		return EBTNodeResult::Failed;
	}

	FOnMontageEnded MT_EndDelegate;
	MT_EndDelegate.BindLambda([this, &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	Enemy->MontagePlay_SetEndDelegate(Montage, MT_EndDelegate);

	return EBTNodeResult::InProgress;
}
