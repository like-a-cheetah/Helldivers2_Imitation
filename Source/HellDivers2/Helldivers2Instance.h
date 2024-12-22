// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Stratagem/StratagemData.h"

#include "Helldivers2Instance.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UHelldivers2Instance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	TArray<TObjectPtr<class UStratagemData>> TempStratagemsD;

public:
	FORCEINLINE void SetTempStratagemD(UStratagemData* InData) { TempStratagemsD.Add(DuplicateObject<UStratagemData>(InData, this)); }
	FORCEINLINE TArray<UStratagemData*> GetTempStratagemsD() { return TempStratagemsD; }
};
