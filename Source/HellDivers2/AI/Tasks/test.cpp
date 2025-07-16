// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/test.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/AnimMovementInterface.h"
#include "Interface/EnemyAIInterface.h"
#include "Characters/Components/EnemyKey.h"
#include "NavigationPath.h"


Utest::Utest()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	ArriveDist = 0.0f;
}

EBTNodeResult::Type Utest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bFollowPath = false;
	NextPathN = 1;

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!Target.IsSet()) return EBTNodeResult::Failed;

	Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	//BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Blackboard->GetBrainComponent());
	//if (!BehaviorTreeComp) return EBTNodeResult::Failed;

	Blackboard->UnregisterObserver(Target.GetSelectedKeyID(), BBObserverHandle);
	BBObserverHandle = Blackboard->RegisterObserver(
		Target.GetSelectedKeyID(),
		this,
		FOnBlackboardChangeNotification::CreateUObject(this, &Utest::OnBlackboardValueChange)
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

		//if (MoveCondition == 0) EnemyMovement->SetWalk();
		//else if (MoveCondition == 1) EnemyMovement->SetTrace();
		if (!EnemyInterface->GetTarget()) EnemyMovement->SetWalk();
		else
		{
			EnemyMovement->SetTrace();
			TargetPlayer = EnemyInterface->GetTarget();
		}
	}

	AgentProps.AgentHeight = 50.0f;
	AgentProps.AgentStepHeight = 50.0f;
	AgentProps.AgentRadius = EnemyInterface->GetBodyRadius() + 10.0f;
	AttackRadius = EnemyInterface->GetBodyRadius() - 50;

	FindPath();

	if (PathPoints.Num() > 1)
	{
		Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
	}

	return EBTNodeResult::InProgress;
}

void Utest::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Target.ResolveSelectedKey(*BBAsset);
	}
}

void Utest::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bRotateClimb)
	{
		FRotator InterpRot = FMath::RInterpTo(ControlledPawn->GetActorRotation(), ClimbRot, DeltaSeconds, 10.f);
		if (InterpRot.Equals(ClimbRot, 10.0f))
		{
			ControlledPawn->SetActorRotation(ClimbRot);
			OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			bRotateClimb = false;
			bClimb = true;
		}
		else ControlledPawn->SetActorRotation(InterpRot);

		return;
	}
	
	if (bEndClimb)
	{

	}


	if (bClimb)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(ControlledPawn);

		FVector Start = ControlledPawn->GetActorLocation();
		FVector Forward = ControlledPawn->GetActorUpVector() * -1.f;
		FVector End = Start + Forward * 200.0f;
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue);

		FHitResult HitResult;

		bool bHit = ControlledPawn->GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		if (!bHit)
		{
			FVector Start2 = End;
			FVector Forward2 = ControlledPawn->GetActorForwardVector() * -1.f;
			FVector End2 = Start2 + Forward2 * 200.0f;
			DrawDebugLine(GetWorld(), Start2, End2, FColor::Blue);

			FHitResult HitResult2;

			bool bHit2 = ControlledPawn->GetWorld()->LineTraceSingleByChannel(
				HitResult2,
				Start2,
				End2,
				ECC_Visibility,
				Params
			);

			if (bHit2)
			{
				FVector Normal = HitResult2.ImpactNormal;
				FRotator temp = FRotationMatrix::MakeFromZ(Normal).Rotator();
				temp.Yaw += 90.0f;

				bClimb = false;
				bEndClimb = true;

				OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				ControlledPawn->SetActorRotation(temp);
			}
		}
	}

	if (bFollowPath)
	{
		if (NextPathN < PathPoints.Num() && ControlledPawn->GetActorLocation().Equals(PathPoints[NextPathN], AttackRadius))
		{
			NextPathN++;

			if (TargetPlayer)
			{
				FVector Start = ControlledPawn->GetActorLocation();
				FVector Forward = ControlledPawn->GetActorForwardVector();
				FVector End = Start + Forward * 200.0f;
				DrawDebugLine(GetWorld(), Start, End, FColor::Blue);

				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(ControlledPawn);

				bool bHit = ControlledPawn->GetWorld()->LineTraceSingleByChannel(
					HitResult,
					Start,
					End,
					ECC_Visibility,
					Params
				);

				if (bHit && HitResult.ImpactNormal.Z < 0.3f)
				{
					bRotateClimb = true;
					Blackboard->SetValueAsBool(BBKEY_CLIMB, true);

					FVector Normal = HitResult.ImpactNormal;
					ClimbRot = FRotationMatrix::MakeFromY(Normal).Rotator();
					ClimbRot.Yaw -= 90.0f;

					OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
				}

				return;
			}

			if (NextPathN >= PathPoints.Num())
			{
				EnemyMovement->SetStop();
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else if (NextPathN < PathPoints.Num())
			{
				Blackboard->SetValueAsVector(BBKEY_NAVPOS, PathPoints[NextPathN].Location);
			}
		}
	}

	//CalculateDistToTarget(OwnerComp);
}

void Utest::CalculateDistToTarget(UBehaviorTreeComponent& OwnerComp)
{
	float DistToTarget = FVector::Distance(ControlledPawn->GetActorLocation(), Blackboard->GetValueAsVector(Target.SelectedKeyName));
	if (DistToTarget <= ArriveDist)
	{
		EnemyMovement->SetStop();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void Utest::FindPath()
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

EBlackboardNotificationResult Utest::OnBlackboardValueChange(const UBlackboardComponent& BlackboardComp, FBlackboard::FKey ChangedKeyID)
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
