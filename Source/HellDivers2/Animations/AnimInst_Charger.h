// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/AnimInst_Enemy.h"

#include "Characters/Monsters/Enemy.h"

#include "AnimInst_Charger.generated.h"

/**
 * 
 */

//DECLARE_DELEGATE(FOnAttackEnded);

UCLASS()
class HELLDIVERS2_API UAnimInst_Charger : public UAnimInst_Enemy
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharger> Charger;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bEndRush : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bHeading : 1;
};
