// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_SetStateMoveTo.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_SetStateMoveTo::UBTTask_SetStateMoveTo()
{
	bNotifyTick = true; 
}

void UBTTask_SetStateMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		const FNavPathSharedPtr Path = AIController->GetPathFollowingComponent()->GetPath();
		if (Path.IsValid())
		{
			const TArray<FNavPathPoint>& Points = Path->GetPathPoints();

			for (const FNavPathPoint& Point : Points)
			{
				DrawDebugSphere(GetWorld(), Point.Location, 40, 8, FColor::Green, false, 0.1f);
			}
		}
	}

	TObjectPtr<class UNavigationSystemV1> NavSys;
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	TObjectPtr<APawn> ControlledPawn;
	ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	FVector OwnerLoc = ControlledPawn->GetActorLocation();

	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* TargetActor = Cast<AActor>(KeyValue);

	FVector GoalPos = TargetActor->GetActorLocation();

	TArray<FNavPathPoint> PathPoints;

	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (NavData)
	{
		FPathFindingQuery Query;
		Query = FPathFindingQuery(
			ControlledPawn,
			*NavData,
			OwnerLoc,
			GoalPos
		);

		FNavAgentProperties AgentProps;

		FPathFindingResult Result = NavSys->FindPathSync(AgentProps, Query);

		if (Result.IsSuccessful())
		{
			PathPoints = Result.Path->GetPathPoints();
		}
	}

	for (FNavPathPoint PathPoint : PathPoints)
		DrawDebugSphere(GetWorld(), PathPoint.Location, 20.0f, 12, FColor::Green, false, 0.5f, 0);
}