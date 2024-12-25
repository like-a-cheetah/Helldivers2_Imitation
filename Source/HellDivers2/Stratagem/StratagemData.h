// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StratagemData.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnActiveStratagemDelegate, bool /*bActive*/);
DECLARE_DELEGATE_OneParam(FOnCorrectMacro, int /*IndexN*/);

UCLASS()
class HELLDIVERS2_API UStratagemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AStratagem> CStratagem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolTime;
	//1 = A, 2 = D, 3 = S, 4 = W
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<uint8> Macro;

	FOnActiveStratagemDelegate OnActiveWidget;
	FOnCorrectMacro OnCorrectMacro;

	void SetbActive(bool InbActive);
	FORCEINLINE bool IsActive() { return bActive; }

	void CorrectMacro(int n);

private:
	uint8 bActive : 1;
};
