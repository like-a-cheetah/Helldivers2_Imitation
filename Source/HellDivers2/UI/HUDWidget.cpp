// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDWidget.h"

#include "Interface/CharacterHUDInterface.h"
#include "PlayerStatWidget.h"
#include "W_StratagemNotice.h"

UHUDWidget::UHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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
}

void UHUDWidget::UpdateStat(int32 GrenadeN, int32 MagazineN, int32 SyringeN)
{
	PlayerStatWidget->SetAllItemNTextBlock(GrenadeN, MagazineN, SyringeN);
}

void UHUDWidget::UpdateGrenadeN(int32 GrenadeN)
{
	PlayerStatWidget->SetGrenadeNTextBlock(GrenadeN);
}

void UHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	PlayerStatWidget->SetHp(NewCurrentHp);
}

void UHUDWidget::ActiveStratagemWidget(bool bActive)
{
	if(bActive) PlayAnimation(ActiveStratagemAnimation);
	else PlayAnimation(ActiveStratagemAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
}

//void UHUDWidget::CreateInformWidget(FString Key, FString Inform)
//{
//	UE_LOG(LogTemp, Log, TEXT("%s"), *Inform);
//	UTextBlock* InformTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
//	InformTextBlock->SetText(FText::FromString(Inform));
//
//	Canvas->AddChild(InformTextBlock);
//}
