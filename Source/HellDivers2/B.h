// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A.h"
#include "B.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AB : public AA
{
	GENERATED_BODY()
	
public:
	AB();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp;
};
