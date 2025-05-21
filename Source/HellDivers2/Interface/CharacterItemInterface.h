// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API ICharacterItemInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(class AItem* Item) = 0;
};
