// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraData.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UCameraData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCameraData();

	UPROPERTY(EditAnywhere, Category = CameraBoom)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = CameraBoom)
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, Category = FollowCamera)
	FRotator Rot;
};
