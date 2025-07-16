// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_MoveTo_Sequence.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "NavigationPath.h"

#include "Interface/AnimMovementInterface.h"
#include "Interface/EnemyAIInterface.h"
#include "Characters/Components/EnemyKey.h"

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
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	BehaviorComp = OwnerComp;

	bFollowPath = false;
	NextPathN = 1;

	if(!Target.IsSet()) return EBTNodeResult::Failed;

	Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	//BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Blackboard->GetBrainComponent());
	//if (!BehaviorTreeComp) return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return EBTNodeResult::Failed;

	ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	EnemyMovement = Cast<IAnimMovementInterface>(ControlledPawn);
	IEnemyAIInterface* EnemyInterface = Cast<IEnemyAIInterface>(ControlledPawn);
	if (EnemyInterface && EnemyMovement)
	{
		TargetPlayer = EnemyInterface->GetTarget();

		AgentProps = ControlledPawn->GetNavAgentPropertiesRef();

		if (!TargetPlayer) EnemyMovement->SetWalk();
		else EnemyMovement->SetTrace();
	}

	if (TargetPlayer)
	{
		Blackboard->UnregisterObserver(Target.GetSelectedKeyID(), BBObserverHandle);
		BBObserverHandle = Blackboard->RegisterObserver(
			Target.GetSelectedKeyID(),
			this,
			FOnBlackboardChangeNotification::CreateUObject(this, &UBTT_MoveTo_Sequence::OnBlackboardValueChange)
		);
	}
	
	if (!FindPath())
	{
		return EBTNodeResult::Failed;
	}

	if (PathPoints.Num() > 1)
	{
		Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
	}

	return EBTNodeResult::InProgress;
}

//void UBTT_MoveTo_Sequence::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	if (bFollowPath)
//	{
//		if (NextPathN < PathPoints.Num())
//		{
//			FVector CurLoc =ControlledPawn->GetActorLocation();
//
//			if (CurLoc.Equals(PathPoints[NextPathN], AgentProps.AgentRadius))
//			{
//				NextPathN++;
//
//				if (NextPathN < PathPoints.Num())
//					Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
//				else
//				{
//					EnemyMovement->SetStop();
//
//					if (TargetPlayer)
//					{
//						if (CurLoc.Equals(TargetPlayer->GetActorLocation(), AgentProps.AgentRadius))
//							FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//						else
//							if (!FindPath())
//								FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//
//					}
//					else
//						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//				}
//			}
//
//			if (!CurLoc.Equals(PreLocation, 40.0f))
//			{
//				PreLocation = CurLoc;
//				BlockedTime = 0.0f;
//			}
//			else
//			{
//				BlockedTime += DeltaSeconds;
//
//				if (BlockedTime >= 1.5f)
//				{
//					BlockedTime = 0.0f;
//					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//				}
//			}
//
//		}
//		else if (NextPathN >= PathPoints.Num())
//			if (!FindPath())
//				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
//	}
//}

void UBTT_MoveTo_Sequence::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bFollowPath && NextPathN < PathPoints.Num())
	{
		FVector CurLoc = ControlledPawn->GetActorLocation();

		if (CurLoc.Equals(PathPoints[NextPathN], AgentProps.AgentRadius))
		{
			NextPathN++;

			if (NextPathN < PathPoints.Num())
				Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
			else
			{
				EnemyMovement->SetStop();

				if (!TargetPlayer)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
				else
				{
					if (CurLoc.Equals(TargetPlayer->GetActorLocation(), AgentProps.AgentRadius))
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					else
						if (!FindPath())
							FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
			}
		}
	}
}

//void UBTT_MoveTo_Sequence::CalculateDistToTarget(UBehaviorTreeComponent& OwnerComp)
//{
//	float DistToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), Blackboard->GetValueAsVector(Target.SelectedKeyName));
//	if (DistToTarget <= ArriveDist)
//	{
//		EnemyMovement->SetStop();
//
//		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	}
//}

bool UBTT_MoveTo_Sequence::FindPath()
{
	const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (NavData)
	{
		//TSubclassOf<UNavigationQueryFilter> QueryFilterClass = UNavigationQueryFilter::StaticClass();
		//FSharedConstNavQueryFilter QueryFilter = UNavigationQueryFilter::GetQueryFilter(*NavSys->GetDefaultNavDataInstance(), OwnerChar, QueryFilterClass);

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

			for (FNavPathPoint& PathPoint : PathPoints)
			{
				PathPoint.Location.Z += AgentProps.AgentHeight;
				DrawDebugSphere(GetWorld(), PathPoint.Location, 25.0f, 16, FColor::Cyan, false, 2.f);
			}

			if (!TargetPlayer)
				return true;
			if (PathPoints.Num() == 0 || !PathPoints.Last().Location.Equals(TargetPlayer->GetActorLocation(), AgentProps.AgentRadius))
				return false;
			
			return true;
		}
	}
	
	return false;
}

EBlackboardNotificationResult UBTT_MoveTo_Sequence::OnBlackboardValueChange(const UBlackboardComponent& BlackboardComp, FBlackboard::FKey ChangedKeyID)
{
	// 기존 경로 도착점이 목표 위치와 일치하지 않는 경우
	if (PathPoints.IsEmpty() || (PathPoints.Num() > 1 && !Blackboard->GetValueAsVector(Target.SelectedKeyName).Equals(PathPoints.Last(), AgentProps.AgentRadius)))
	{
		// 경로를 재탐색해, 성공했다면 다음 지점을 설정, 
		if (FindPath())
			Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[1].Location);
		else
			FinishLatentTask(*BehaviorComp, EBTNodeResult::Failed);
	}
	return EBlackboardNotificationResult::ContinueObserving;
}