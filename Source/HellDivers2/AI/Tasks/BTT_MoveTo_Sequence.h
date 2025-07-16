// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveTo_Sequence.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTT_MoveTo_Sequence : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveTo_Sequence();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//void CalculateDistToTarget(UBehaviorTreeComponent& OwnerComp);

	bool FindPath();

private:
	TObjectPtr<class UBlackboardComponent> Blackboard;
	//TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComp;

	class IAnimMovementInterface* EnemyMovement;
	TObjectPtr<APawn> ControlledPawn;

	TObjectPtr<AActor> TargetPlayer;

	FNavAgentProperties AgentProps;
	TObjectPtr<class UNavigationSystemV1> NavSys;
	TObjectPtr<class UNavigationPath> NavPath;
	TArray<FNavPathPoint> PathPoints;
	int NextPathN;

	float TimeSinceLastUpdate = 0.0f;

	FDelegateHandle BBObserverHandle;

	uint8 bFollowPath : 1;

	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	FVector PreLocation;
	float BlockedTime;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ArriveDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	uint8 MoveCondition;

public:
	EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& BlackboardComp, FBlackboard::FKey ChangedKeyID);
};
