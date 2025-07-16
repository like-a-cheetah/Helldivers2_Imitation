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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TObjectPtr<class UPlayerStatWidget> PlayerStatWidget;

public:
	//void UpdateStat(int32 GrenadeN, int32 MagazineN, int32 SyringeN);
	//void UpdateGrenadeN(int32 GrenadeN);
	void UpdateHpBar(float NewCurrentHp);

	void ActiveStratagemWidget(bool bActive);

	void SetActiveCrosshair(bool bActive);

	void PlayEnemyHitAnim();

	void PlayMiniMapExpansion(bool bExpansion);

	void SetActiveHealWidget(bool bHealing);

protected:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UW_StratagemNotice> StratagemNoticeWidget;

private:

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UUserWidget> Crosshair;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UImage> W_Heal;
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UImage> W_Blood;

	FLinearColor W_HealInitColor;
	FLinearColor W_BloodInitColor;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> ActiveStratagemAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_TakeDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_Heal;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_HealEnd;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_CriticalHealth;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_HitEnemy;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> WAnim_ExpansionMinimap;

	float MinimapAnimPlayTime;
};
