// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_ToClimb.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/AnimMovementInterface.h"
#include "Interface/EnemyAIInterface.h"
#include "Characters/Components/EnemyKey.h"

UBTT_ToClimb::UBTT_ToClimb()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_ToClimb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	IAnimMovementInterface* EnemyMovement = Cast<IAnimMovementInterface>(ControlledPawn);
	IEnemyAIInterface* EnemyInterface = Cast<IEnemyAIInterface>(ControlledPawn);
	if (!EnemyMovement || !EnemyInterface) return EBTNodeResult::Failed;

	Target = EnemyInterface->GetTarget();

	EnemyMovement->SetTrace();

	MovementComp = OwnerComp.GetAIOwner()->GetCharacter()->GetCharacterMovement();
	MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);

	bRot = true;

	return EBTNodeResult::InProgress;
}

void UBTT_ToClimb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const FVector ActorLoc = ControlledPawn->GetActorLocation();
	const FVector UpVec = ControlledPawn->GetActorUpVector();
	const FVector ForwardVec = ControlledPawn->GetActorForwardVector();
	const FVector RightVec = ControlledPawn->GetActorRightVector();

	FVector End1 = ActorLoc + ForwardVec * 500.0f + UpVec * -500.0f;
	FHitResult Hit1;
	GetWorld()->LineTraceSingleByObjectType(Hit1, ActorLoc, End1, ECollisionChannel::ECC_WorldStatic);
	DrawDebugLine(GetWorld(), ActorLoc, End1, FColor::Red);

	FVector End2 = ActorLoc + ForwardVec * 600.0f + UpVec * -500.0f;
	FHitResult Hit2;
	GetWorld()->LineTraceSingleByObjectType(Hit2, ActorLoc, End2, ECollisionChannel::ECC_WorldStatic);
	DrawDebugLine(GetWorld(), ActorLoc, End2, FColor::Blue);

	FVector End3 = ActorLoc + ForwardVec * 500.0f + UpVec * -500.0f + RightVec * 100.f;
	FHitResult Hit3;
	GetWorld()->LineTraceSingleByObjectType(Hit3, ActorLoc, End3, ECollisionChannel::ECC_WorldStatic);
	DrawDebugLine(GetWorld(), ActorLoc, End3, FColor::Green);

	FVector TargetLoc = Hit1.Location + Hit1.Normal * 60.0f;

	const FVector Forward = (Hit2.Location - Hit1.Location).GetSafeNormal();
	const FVector Right = (Hit3.Location - Hit1.Location).GetSafeNormal();
	const FVector Up = Hit1.Normal;

	FMatrix RotationMatrix(
		FPlane(Forward.X, Forward.Y, Forward.Z, 0.f),
		FPlane(Right.X, Right.Y, Right.Z, 0.f),
		FPlane(Up.X, Up.Y, Up.Z, 0.f),
		FPlane(0.f, 0.f, 0.f, 1.f)
	);

	if (Hit1.bBlockingHit || Hit2.bBlockingHit || Hit3.bBlockingHit)
	{
		Rotation = RotationMatrix.Rotator();
		UE_LOG(LogTemp, Log, TEXT("%s"), *Rotation.ToString());
	}

	FRotator InterpRot = FMath::RInterpTo(ControlledPawn->GetActorRotation(), Rotation, DeltaSeconds, 3.f);

	ControlledPawn->SetActorRotation(InterpRot);
 }
