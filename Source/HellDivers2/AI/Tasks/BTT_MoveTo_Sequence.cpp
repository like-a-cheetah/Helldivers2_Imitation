// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_MoveTo_Sequence.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "Interface/AnimMovementInterface.h"
#include "Interface/EnemyAIInterface.h"
#include "Characters/Components/EnemyKey.h"
#include "NavigationPath.h"

UBTT_MoveTo_Sequence::UBTT_MoveTo_Sequence()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	ArriveDist = 0.0f;
}

void UBTT_MoveTo_Sequence::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Target.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTT_MoveTo_Sequence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bFollowPath = false;
	NextPathN = 1;

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if(!Target.IsSet()) return EBTNodeResult::Failed;

	Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	//BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Blackboard->GetBrainComponent());
	//if (!BehaviorTreeComp) return EBTNodeResult::Failed;

	Blackboard->UnregisterObserver(Target.GetSelectedKeyID(), BBObserverHandle);
	BBObserverHandle = Blackboard->RegisterObserver(
		Target.GetSelectedKeyID(),
		this,
		FOnBlackboardChangeNotification::CreateUObject(this, &UBTT_MoveTo_Sequence::OnBlackboardValueChange)
	);

	UAnimInstance* AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	EnemyMovement = Cast<IAnimMovementInterface>(ControlledPawn);
	IEnemyAIInterface* EnemyInterface = Cast<IEnemyAIInterface>(ControlledPawn);
	if (EnemyInterface && EnemyMovement)
	{
		ArriveDist = EnemyInterface->GetBodyRadius();

		if (MoveCondition == 0) EnemyMovement->SetWalk();
		else if (MoveCondition == 1) EnemyMovement->SetTrace();
	}

	AgentProps.AgentRadius = EnemyInterface->GetBodyRadius() + 10.0f;
	AttackRadius = EnemyInterface->GetBodyRadius() - 50;

	FindPath();

	if (PathPoints.Num() > 1)
	{
		Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
	}

	return EBTNodeResult::InProgress;
}

void UBTT_MoveTo_Sequence::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bFollowPath)
	{
		if (NextPathN < PathPoints.Num() && ControlledPawn->GetActorLocation().Equals(PathPoints[NextPathN], AttackRadius))
		{
			NextPathN++;

			if (NextPathN < PathPoints.Num())
			{
				Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
			}
			else
			{
				EnemyMovement->SetStop();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	//CalculateDistToTarget(OwnerComp);
}

void UBTT_MoveTo_Sequence::CalculateDistToTarget(UBehaviorTreeComponent& OwnerComp)
{
	float DistToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), Blackboard->GetValueAsVector(Target.SelectedKeyName));
	if (DistToTarget <= ArriveDist)
	{
		EnemyMovement->SetStop();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_MoveTo_Sequence::FindPath()
{
	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (NavData)
	{
		FPathFindingQuery Query;
		Query = FPathFindingQuery(
			ControlledPawn,
			*NavData,
			ControlledPawn->GetActorLocation(),
			Blackboard->GetValueAsVector(Target.SelectedKeyName)
		);

		FPathFindingResult Result = NavSys->FindPathSync(AgentProps, Query);

		if (Result.IsSuccessful())
		{
			bFollowPath = true;

			NextPathN = 1;

			PathPoints = Result.Path->GetPathPoints();

			for (FNavPathPoint PathPoint : PathPoints)
				DrawDebugSphere(GetWorld(), PathPoint.Location, 25.0f, 16, FColor::Cyan, false, 3.0f);
		}
	}
}

EBlackboardNotificationResult UBTT_MoveTo_Sequence::OnBlackboardValueChange(const UBlackboardComponent& BlackboardComp, FBlackboard::FKey ChangedKeyID)
{
	if (PathPoints.Num() > 1 && !Blackboard->GetValueAsVector(Target.SelectedKeyName).Equals(PathPoints.Last(), AttackRadius))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Target Location Changed"));

		//StopWaitingForMessages(*BehaviorTreeComp);

		FindPath();

		if (PathPoints.Num() > 1) Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[1].Location);

		//const EBTNodeResult::Type NewResult = PerformMoveTask(*BehaviorTreeComp, NodeMemory);
		//if (NewResult != EBTNodeResult::InProgress)
		//{
		//	FinishLatentTask(*BehaviorTreeComp, NewResult);
		//}
	}

	return EBlackboardNotificationResult::ContinueObserving;
}
