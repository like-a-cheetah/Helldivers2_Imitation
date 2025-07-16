// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyKey.h"

#include "AIController_Enemy.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnPawnTargetUpdate, AActor* /*InTarget*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetectedSignature, AActor*, EnemyActor);

UCLASS()
class HELLDIVERS2_API AAIController_Enemy : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Enemy();
	void RunAI();
	void StopAI();
	void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	void CallTeam();

	TArray<AAIController_Enemy*> GetTeamAIs() const;

	UFUNCTION()
	virtual void SetTarget(AActor* Actor);
	virtual void ReceiveRallyCall(AActor* InTarget);

	FORCEINLINE void SetSmoothRotate(bool InbSmoothRotate) { bSmoothRotate = InbSmoothRotate; }
	FORCEINLINE bool IsSmoothRotate() { return bSmoothRotate; }

private:
	void SetupPerceptionSystem();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Hearing> HearConfig;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Team> TeamConfig;

	UPROPERTY()
	TObjectPtr<AActor> Target;

	FOnPawnTargetUpdate OnPawnTargetUpdate;

	FGenericTeamId TeamID;

	FOnEnemyDetectedSignature OnEnemyDetected;

	uint8 bSmoothRotate : 1;

protected:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

protected:
	UFUNCTION()
	virtual void ProcessStimulus(AActor* Actor, FAIStimulus Stimulus);

private:
	void CalculateDist();

	////IGenericTeamAgentInterface
//public:
	//FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& InTeamId) override { TeamID = InTeamId; }
	//FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId::NoTeam; }
	//FORCEINLINE virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const
	//{
	//	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	//	return OtherTeamAgent ? FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId())
	//		: ETeamAttitude::Neutral;
	//}
};
