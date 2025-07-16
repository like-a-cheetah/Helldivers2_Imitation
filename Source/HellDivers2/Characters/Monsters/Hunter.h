// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Monsters/Enemy.h"

#include "Interface/AcidCompInterface.h"

#include "Hunter.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHunter : public AEnemy, public IAcidCompInterface
{
	GENERATED_BODY()
	
public:
	AHunter();

	// IAcidCompInterface을(를) 통해 상속됨
	UAcidComponent* GetAcidComp() override;
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void BeginActivity() override;
};
