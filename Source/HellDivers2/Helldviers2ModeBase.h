// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Helldviers2ModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHelldviers2ModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHelldviers2ModeBase();

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Controller) override;
	virtual void BeginPlay() override;

private:
	FVector StartPos;
	FRotator StartRot;
};
