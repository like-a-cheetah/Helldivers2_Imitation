// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/HellPod.h"
#include "Hellpod_Resupply.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHellpod_Resupply : public AHellpod
{
	GENERATED_BODY()
	
public:
	AHellpod_Resupply();

	virtual void BeginPlay() override;

private:
	void SpawnAttachment();

private:
	UPROPERTY()
	TObjectPtr<UAnimationAsset> DefaultAnim;
	UPROPERTY()
	TObjectPtr<UAnimationAsset> SpreadAnim;
	UPROPERTY()
	TObjectPtr<UAnimationAsset> FoldAnim;
};
