// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A.generated.h"

UCLASS()
class HELLDIVERS2_API AA : public AActor
{
	GENERATED_BODY()
	
public:	
	AA();

	TObjectPtr<USkinnedMeshComponent> SkinnedMeshComp;

};
