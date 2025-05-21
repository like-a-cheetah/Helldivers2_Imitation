// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EnableAttackBoneCollision.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UAN_EnableAttackBoneCollision : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BoneName")
	FName BoneName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BoneName")
	uint8 bActive : 1;

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
