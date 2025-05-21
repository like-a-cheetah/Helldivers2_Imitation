// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_BB_SetRandVar.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTS_BB_SetRandVar::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
    {
        return;
    }

    Blackboard->SetValueAsInt(Var.SelectedKeyName, rand() % Range);
}
