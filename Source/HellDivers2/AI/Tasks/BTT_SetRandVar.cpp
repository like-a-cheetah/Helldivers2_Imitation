// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_SetRandVar.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_SetRandVar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    BlackboardComp->SetValueAsInt(Var.SelectedKeyName, rand() % Range);

    return EBTNodeResult::Succeeded;
}