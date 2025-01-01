// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stratagem.generated.h"

UENUM(BlueprintType)
enum class EStratagemType : uint8
{
	NotAttack = 0,
	Attack
};

DECLARE_DELEGATE(FOnDestoryBall);

UCLASS()
class HELLDIVERS2_API AStratagem : public AActor
{
	GENERATED_BODY()
	
public:	
	AStratagem();

	FOnDestoryBall OnDestoryBall;

protected:
	//TSubclassOf<class ARocket>
};
