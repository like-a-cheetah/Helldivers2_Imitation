// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StratagemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UStratagemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IStratagemInterface
{
	GENERATED_BODY()

public:
	virtual void SetPlayerStratagem(class UStratagemData* SData) = 0;
};
