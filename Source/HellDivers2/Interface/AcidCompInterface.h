// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Characters/Components/AcidComponent.h"

#include "AcidCompInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAcidCompInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IAcidCompInterface
{
	GENERATED_BODY()

protected:
	TObjectPtr<UAcidComponent> AcidComp;

public:
	virtual UAcidComponent* GetAcidComp() = 0;
};
