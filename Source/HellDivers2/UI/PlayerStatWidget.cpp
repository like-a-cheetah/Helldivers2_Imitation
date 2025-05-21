// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));
	ensure(HpProgressBar);

	GrenadeNTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("GrenadeN")));
	ensure(GrenadeNTextBlock);

	MagazineNTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("MagazineN")));
	ensure(MagazineNTextBlock);

	SyringeNTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("SyringeN")));
	ensure(SyringeNTextBlock);
}

float UPlayerStatWidget::GetHp()
{
	return HpProgressBar->GetPercent();
}

void UPlayerStatWidget::SetHp(float CurrentHp)
{
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp);
	}
}

void UPlayerStatWidget::SetAllItemNTextBlock(int32 GrenadeN, int32 MagazineN, int32 SyringeN)
{
	if (GrenadeNTextBlock)	GrenadeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(GrenadeN)));
	if (MagazineNTextBlock) MagazineNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(MagazineN)));
	if (SyringeNTextBlock)	SyringeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(SyringeN)));
}

void UPlayerStatWidget::SetGrenadeNTextBlock(int32 GrenadeN)
{
	if (GrenadeNTextBlock)	GrenadeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(GrenadeN)));
}
