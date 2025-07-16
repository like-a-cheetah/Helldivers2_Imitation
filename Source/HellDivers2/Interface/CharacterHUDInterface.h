// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API ICharacterHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetupHUDWidget(class UUserWidget* InHUDWidget) = 0;
	virtual void LoadBattleFieldLevel() = 0;
	virtual void SetStratagemsNoticeWidget(UUserWidget* InStratagemNoticeWidget) = 0;
	virtual void SetStratagemConditionWidget(UUserWidget* InStratagemNoticeWidget) = 0;
};
