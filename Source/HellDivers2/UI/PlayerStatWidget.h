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

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UImage> Img_GunIcon;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PrBar_Round;

public:
	UFUNCTION()
	float GetHp();
	UFUNCTION()
	void SetHp(float CurrentHp);
	void SetAllItemCondition(int32 GrenadeN, int32 MagazineN, int32 SyringeN, class IGunInterface* Gun);
	UFUNCTION()
	void SetGrenadeNTextBlock(int32 GrenadeN);

	void RoundChange(float Ratio);
};
