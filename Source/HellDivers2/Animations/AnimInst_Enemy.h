// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Characters/Monsters/Enemy.h"

#include "AnimInst_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UAnimInst_Enemy : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInst_Enemy();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AEnemy> Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	EEnemyMovementMode EnemyMovementMode;

	//몬스터 애님인스턴스 부모 클래스 만들어서 거기에 구현하기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float RotateAngle;

	void SetLegRig(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "IK")
	float LegRadius;

	UPROPERTY(BlueprintReadWrite, Category = "IK")
	float KneeZ;

	UPROPERTY(BlueprintReadWrite, Category = "IK")
	FName FRLegName;
	UPROPERTY(BlueprintReadWrite, Category = "IK")
	FName FLLegName;
	UPROPERTY(BlueprintReadWrite, Category = "IK")
	FName BRLegName;
	UPROPERTY(BlueprintReadWrite, Category = "IK")
	FName BLLegName;

	UPROPERTY(BlueprintReadOnly, Category = "IK")
	float FRLegGoalZ;
	UPROPERTY(BlueprintReadOnly, Category = "IK")
	float FLLegGoalZ;
	UPROPERTY(BlueprintReadOnly, Category = "IK")
	float BRLegGoalZ;
	UPROPERTY(BlueprintReadOnly, Category = "IK")
	float BLLegGoalZ;

public:
	FORCEINLINE void SetRotateAngle(float InAngle) { RotateAngle = InAngle; }
};
