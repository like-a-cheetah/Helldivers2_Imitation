// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

#include "Interface/CharacterHUDInterface.h"
#include "PlayerStatWidget.h"
#include "W_StratagemNotice.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	MinimapAnimPlayTime += InDeltaTime;
}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerStatWidget = Cast<UPlayerStatWidget>(GetWidgetFromName(TEXT("WidgetPlayerStat")));
	ensure(PlayerStatWidget);
	
	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
		HUDPawn->SetStratagemsNoticeWidget(StratagemNoticeWidget);
	}

	W_BloodInitColor = W_Blood->GetColorAndOpacity();
	W_BloodInitColor.A = 0;
	W_HealInitColor = W_Heal->GetColorAndOpacity();
	W_HealInitColor.A = 0;
}

//void UHUDWidget::UpdateStat(int32 GrenadeN, int32 MagazineN, int32 SyringeN)
//{
//	PlayerStatWidget->SetAllItemCondition(GrenadeN, MagazineN, SyringeN);
//}

//void UHUDWidget::UpdateGrenadeN(int32 GrenadeN)
//{
//	PlayerStatWidget->SetGrenadeNTextBlock(GrenadeN);
//}

void UHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	NewCurrentHp = NewCurrentHp / 100.0f;
	float PreHP = PlayerStatWidget->GetHp();

	if (PreHP > NewCurrentHp && (!IsAnimationPlaying(WAnim_Heal) && W_Heal->GetColorAndOpacity().A != 1))
	{
		if(NewCurrentHp > 0.5f) PlayAnimation(WAnim_TakeDamage);
		else PlayAnimation(WAnim_CriticalHealth, 0.0f, 9999999);
	}
	else if (PreHP < NewCurrentHp && NewCurrentHp > 0.5f)
	{
		StopAnimation(WAnim_CriticalHealth);
		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(Tmp, [this]() {
			W_Blood->SetColorAndOpacity(W_BloodInitColor);
			},
			.1f, false);
	}


	PlayerStatWidget->SetHp(NewCurrentHp);
}

void UHUDWidget::ActiveStratagemWidget(bool bActive)
{
	if (bActive)
	{
		PlayAnimation(ActiveStratagemAnimation);
	}
	else
	{
		PlayAnimation(ActiveStratagemAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UHUDWidget::SetActiveCrosshair(bool bActive)
{
	if(bActive) Crosshair->SetVisibility(ESlateVisibility::Visible);
	else Crosshair->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::PlayEnemyHitAnim()
{
	PlayAnimation(WAnim_HitEnemy);
}

void UHUDWidget::PlayMiniMapExpansion(bool bExpansion)
{
	if (bExpansion)
	{
		MinimapAnimPlayTime = 0.0f;
		PlayAnimation(WAnim_ExpansionMinimap);
	}
	else
	{
		float PlayStartTime = WAnim_ExpansionMinimap->GetEndTime() - MinimapAnimPlayTime;
		PlayStartTime = FMath::Clamp(PlayStartTime, 0.0f, WAnim_ExpansionMinimap->GetEndTime());
		PlayAnimation(WAnim_ExpansionMinimap, PlayStartTime, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UHUDWidget::SetActiveHealWidget(bool bHealing)
{
	if (bHealing)
	{
		StopAnimation(WAnim_TakeDamage);
		StopAnimation(WAnim_CriticalHealth);

		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(Tmp, [this]() {
			W_Blood->SetColorAndOpacity(W_BloodInitColor);
			},
			.1f,     // 1초 후
			false     // 반복 X (한 번만 실행)
		);

		PlayAnimation(WAnim_Heal);
	}
	else PlayAnimation(WAnim_HealEnd);
}

//void UHUDWidget::CreateInformWidget(FString Key, FString Inform)
//{
//	UE_LOG(LogTemp, Log, TEXT("%s"), *Inform);
//	UTextBlock* InformTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
//	InformTextBlock->SetText(FText::FromString(Inform));
//
//	Canvas->AddChild(InformTextBlock);
//}
