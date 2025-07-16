// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Items/EItemType.h"
#include "Characters/Player/EPose.h"

#include "PlayerAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IPlayerAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EItemType GetCurrentItemType() = 0;
	virtual EPose GetCurrentPose() = 0;
	virtual bool IsRightButton() = 0;
	virtual bool IsLeftButton() = 0;
	virtual bool IsSucceededStratagem() = 0;
	virtual bool IsPullingPin() = 0;
	virtual bool IsConsoleActive() = 0;
	virtual bool IsRegIkActive() = 0;
	virtual FVector2D GetInputVector() = 0;
	virtual FRotator GetChestRot() = 0;
};
