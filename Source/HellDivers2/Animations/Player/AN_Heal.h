// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Heal.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UAN_Heal : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
};
