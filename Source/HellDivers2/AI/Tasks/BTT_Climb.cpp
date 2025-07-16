// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_Climb.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}


	return EBTNodeResult::InProgress;
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector StartLoc = ControlPawn->GetActorLocation();
	FVector EndLoc = StartLoc + ControlPawn->GetActorForwardVector() * 100.0f;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, StartLoc, EndLoc, ECC_WorldStatic);
	if (HitResult.bBlockingHit)
	{

	}
}
