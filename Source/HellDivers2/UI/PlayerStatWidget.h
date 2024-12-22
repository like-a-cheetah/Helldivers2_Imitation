// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	UPROPERTY()
	TObjectPtr<class UTextBlock> GrenadeNTextBlock;
	UPROPERTY()
	TObjectPtr<class UTextBlock> MagazineNTextBlock;
	UPROPERTY()
	TObjectPtr<class UTextBlock> SyringeNTextBlock;

public:
	UFUNCTION()
	void SetHp(float CurrentHp);
	UFUNCTION()
	void SetAllItemNTextBlock(int32 GrenadeN, int32 MagazineN, int32 SyringeN);
	UFUNCTION()
	void SetGrenadeNTextBlock(int32 GrenadeN);
};
