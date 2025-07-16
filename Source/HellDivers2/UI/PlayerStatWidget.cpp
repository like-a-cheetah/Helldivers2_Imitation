// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBoxSlot.h"

#include "Interface/GunInterface.h"

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

void UPlayerStatWidget::SetAllItemCondition(int32 GrenadeN, int32 MagazineN, int32 SyringeN, IGunInterface* Gun)
{
	if (GrenadeNTextBlock)	GrenadeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(GrenadeN)));
	if (MagazineNTextBlock) MagazineNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(MagazineN)));
	if (SyringeNTextBlock)	SyringeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(SyringeN)));

	if (Gun)
	{
		FSlateBrush Brush = Img_GunIcon->GetBrush();

		UTexture* NewIcon = Gun->GetGunIcon();
		Brush.SetResourceObject(NewIcon);

		FVector2D IconSize(NewIcon->GetResource()->GetSizeX(), NewIcon->GetResource()->GetSizeY());
		UE::Slate::FDeprecateVector2DParameter WidgetImageSize(IconSize);
		Brush.SetImageSize(WidgetImageSize);

		Img_GunIcon->SetBrush(Brush);


		if (Gun->GetRoundIcon())
		{
			FProgressBarStyle PrBarStyle = PrBar_Round->GetWidgetStyle();

			Brush.SetResourceObject(Gun->GetRoundIcon());
			PrBarStyle.SetBackgroundImage(Brush);
			if (Gun->GetRoundFillIcon())
			{
				Brush.SetResourceObject(Gun->GetRoundFillIcon());
				PrBarStyle.SetFillImage(Brush);
			}

			PrBar_Round->SetWidgetStyle(PrBarStyle);
		}


		RoundChange(Gun->GetRoundRatio());

		//Brush.SetResourceObject(RoundIcon);
		//PrBarStyle.SetFillImage(Brush);
	}
}

void UPlayerStatWidget::SetGrenadeNTextBlock(int32 GrenadeN)
{
	if (GrenadeNTextBlock)	GrenadeNTextBlock->SetText(FText::Format(FText::FromString("x{0}/4"), FText::AsNumber(GrenadeN)));
}

void UPlayerStatWidget::RoundChange(float Ratio)
{
	PrBar_Round->SetPercent(Ratio);
}
