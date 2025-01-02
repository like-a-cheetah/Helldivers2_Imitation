// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Stratagem.h"

#include "StratagemData.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnActiveStratagemDelegate, bool /*bActive*/);
DECLARE_DELEGATE_OneParam(FOnCorrectMacro, int /*IndexN*/);
DECLARE_DELEGATE_OneParam(FOnSetCoolTimeText, FString /*Str*/);
DECLARE_DELEGATE_OneParam(FOnHideMacroBox, bool /*bHide*/);
DECLARE_DELEGATE_OneParam(FOnShowConditionWidget, bool /*bShow*/);

UCLASS()
class HELLDIVERS2_API UStratagemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AStratagem> CStratagem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCoolTime : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCoolTime;
	float CoolTime;

	//1 = ก็, 2 = กๆ, 3 = ก้, 4 = ก่
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<uint8> Macro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	EStratagemType StratagemType;

	FOnSetCoolTimeText OnSetCoolTimeText;
	FOnActiveStratagemDelegate OnActiveWidget;
	FOnCorrectMacro OnCorrectMacro;
	FOnHideMacroBox OnHideMacroBox;
	FOnShowConditionWidget ShowConditionWidgetDelegate;

	void SetbActive(bool InbActive);
	FORCEINLINE bool IsActive() { return bActive; }

	void CorrectMacro(int n);

	void SetCoolTime(UWorld* World);

	FORCEINLINE TSubclassOf<AStratagem> GetCStratagem() { return CStratagem; }
	FORCEINLINE EStratagemType GetStratagemType() { return StratagemType; }

private:
	uint8 bActive : 1;
};
