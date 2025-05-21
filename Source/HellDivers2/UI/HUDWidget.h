// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(int32 GrenadeN, int32 MagazineN, int32 SyringeN);
	void UpdateGrenadeN(int32 GrenadeN);
	void UpdateHpBar(float NewCurrentHp);

	void ActiveStratagemWidget(bool bActive);

	void PlayEnemyHitAnim();

protected:
	virtual void NativeConstruct() override;

protected:
	TObjectPtr<class UPlayerStatWidget> PlayerStatWidget;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UW_StratagemNotice> StratagemNoticeWidget;

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> ActiveStratagemAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_TakeDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_Heal;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_CriticalHealth;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_OffDamageEffect;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_HitEnemy;
};
