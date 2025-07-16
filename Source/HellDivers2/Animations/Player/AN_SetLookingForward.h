// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SetLookingForward.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UAN_SetLookingForward : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bActive : 1;

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
