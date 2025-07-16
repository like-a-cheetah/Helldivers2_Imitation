// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInst_Enemy.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/Monsters/Enemy.h"

UAnimInst_Enemy::UAnimInst_Enemy()
{
	LegRadius = 20.0f;
	KneeZ = 30.0;
}

void UAnimInst_Enemy::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Enemy = Cast<AEnemy>(Owner);
		Movement = Owner->GetCharacterMovement();

		EnemyMovementMode = Cast<AEnemy>(Owner)->GetEnemyMovementMode();
	}
}

void UAnimInst_Enemy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Enemy)
	{
		EnemyMovementMode = Cast<AEnemy>(Owner)->GetEnemyMovementMode();

		//SetLegRig(DeltaSeconds);
	}
}

void UAnimInst_Enemy::SetLegRig(float DeltaTime)
{
	USkeletalMeshComponent* Mesh = Owner->GetMesh();

	if (EnemyMovementMode == EEnemyMovementMode::Die || Movement->IsFalling())
	{
		FRLegGoalZ = 0;
		FLLegGoalZ = 0;
		BRLegGoalZ = 0;
		BLLegGoalZ = 0;

		return;
	}


	float GroundZ = Mesh->GetBoneLocation(TEXT("root")).Z;
	float TraceStartZ = GroundZ + KneeZ;
	float TraceEndZ = GroundZ - 150.0f;

	FCollisionShape Shape = FCollisionShape::MakeSphere(LegRadius);

	auto Op_CalculatZ = [this, Mesh, GroundZ, TraceStartZ, TraceEndZ, Shape, DeltaTime](FName BoneName, float& TargetLegZ) {
		FVector LegLoc = Mesh->GetBoneLocation(BoneName);
		
		FVector TraceStartLoc = FVector(LegLoc.X, LegLoc.Y, TraceStartZ);
		FVector TraceEndLoc = FVector(LegLoc.X, LegLoc.Y, TraceEndZ);

		//DrawDebugCylinder(GetWorld(), TraceStartLoc, TraceEndLoc, LegRadius, 4, FColor::Blue);
		FHitResult HitResult;
		bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, TraceStartLoc, TraceEndLoc, FQuat::Identity, RTS_World, Shape);
		if (bHit)
		{
			float Z = HitResult.ImpactPoint.Z - GroundZ;
			TargetLegZ = FMath::FInterpTo(TargetLegZ, Z, DeltaTime, 15.0f);
		}
		else TargetLegZ = 0; 
	};

	Op_CalculatZ(FRLegName, FRLegGoalZ);
	Op_CalculatZ(FLLegName, FLLegGoalZ);
	Op_CalculatZ(BRLegName, BRLegGoalZ);
	Op_CalculatZ(BLLegName, BLLegGoalZ);
}
