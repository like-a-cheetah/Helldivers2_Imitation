// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTS_RotateToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "Characters/Monsters/Enemy.h"
#include "Characters/Components/EnemyKey.h"
#include "Characters/Components/AIController_Enemy.h"

void UBTS_RotateToTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTS_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Target.ResolveSelectedKey(*BBAsset);
	}
}

void UBTS_RotateToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	//if (Blackboard->GetValueAsBool(BBKEY_CLIMB))
	//{
	//	AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);
	//	Enemy->SetRotate(0.0f);
	//	Blackboard->SetValueAsFloat(BBKEY_ROTATE_TARGET, 0.0f);
	//	return;
	//}

	if (Target.IsSet())
	{
		FVector GoalPos;
		if (Target.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			GoalPos = Blackboard->GetValueAsVector(Target.SelectedKeyName);
		}
		else if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* TargetObj = Blackboard->GetValueAsObject(Target.SelectedKeyName);
			GoalPos = Cast<AActor>(TargetObj)->GetActorLocation();
		}

		FVector PawnLocation = ControlledPawn->GetActorLocation();

		FVector Direction = GoalPos - PawnLocation;
		Direction.Z = 0.f;
		if (Direction.IsNearlyZero())
			return;
		Direction.Normalize();

		FVector ForwardVector = ControlledPawn->GetActorForwardVector();
		ForwardVector.Z = 0.f;

		float Dot = FMath::Clamp(FVector::DotProduct(ForwardVector, Direction), -1.0f, 1.0f);
		float Angle = FMath::RadiansToDegrees(acosf(Dot));
		FVector Cross = FVector::CrossProduct(ForwardVector, Direction);

		
		AAIController_Enemy* AICon = Cast<AAIController_Enemy>(OwnerComp.GetAIOwner());

		float RelativeAngle = Cross.Z < 0 ? -Angle : Angle;
		if (abs(RelativeAngle) < MaxRelativeAngle && AICon->IsSmoothRotate())
		{
			FRotator CurrentRotation = ControlledPawn->GetActorRotation();
			FRotator TargetRotation = Direction.Rotation();

			FRotator SmoothRot = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, InterpSpeed);
			ControlledPawn->SetActorRotation(SmoothRot);
		}

		AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);
		Enemy->SetRotate(RelativeAngle);

		Blackboard->SetValueAsFloat(BBKEY_ROTATE_TARGET, RelativeAngle);

		bool bMeleeAttackRange = RelativeAngle <= 45.0f && RelativeAngle >= -45.0f;
		Blackboard->SetValueAsBool(BBKEY_MELEE_ATTACK_RANGE, bMeleeAttackRange);
	}
}