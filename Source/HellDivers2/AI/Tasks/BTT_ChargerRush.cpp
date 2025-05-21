// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_ChargerRush.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Monsters/Charger.h"
#include "Characters/Components/EnemyKey.h"

UBTT_ChargerRush::UBTT_ChargerRush()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ChargerRush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	Charger = Cast<ACharger>(ControlledPawn);
	if (!Charger)
	{
		return EBTNodeResult::Failed;
	}

	Charger->Rush();
	Charger->SetbEndRush(false);
	Charger->SetbHeading(false);

	UWorld* World = ControlledPawn->GetWorld();
	if (!World)
	{
		return EBTNodeResult::Failed;
	}

	bStartedRush = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		([this]()
			{
				UE_LOG(LogTemp, Log, TEXT("bStartedRush"));
				bStartedRush = true;
			}),
		1.0f,
		false
	);

	return EBTNodeResult::InProgress;
}

void UBTT_ChargerRush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bStartedRush) return;

	Duration += DeltaSeconds;

	if (Duration >= 6.0f)
	{
		Charger->SetbEndRush(true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		float MaxCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MAX_RUSH_COOLTIME);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_CURRENT_RUSH_COOLTIME, MaxCoolTime);

		Duration = 0.0f;
		return;
	}
	else
	{
		float TargetDist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TARGET_DIST);
		float RelativeAngle = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_ROTATE_TARGET);
		if (TargetDist <= 480.0f && (RelativeAngle <= 90.0f && RelativeAngle >= -90.0f))
		{
			Charger->SetbEndRush(true);
			Charger->SetbHeading(true);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			float MaxCoolTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_MAX_RUSH_COOLTIME);
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBKEY_CURRENT_RUSH_COOLTIME, MaxCoolTime);

			Duration = 0.0f;
			return;
		}
	}
}