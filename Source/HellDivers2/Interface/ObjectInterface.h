// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IObjectInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Player) = 0;
};
