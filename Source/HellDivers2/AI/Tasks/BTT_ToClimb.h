// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ToClimb.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UBTT_ToClimb : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_ToClimb();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	TObjectPtr<APawn> ControlledPawn;
	TObjectPtr<AActor> Target;

	FRotator StartRot;
	FVector StartLoc;

	FRotator SlopRot;
	FVector SlopLoc;

	uint8 bRot : 1;
	uint8 bFinish : 1;
	
	FRotator Rotation;

	float BodyHalfHeight;

	TObjectPtr<class UCharacterMovementComponent> MovementComp;
};
