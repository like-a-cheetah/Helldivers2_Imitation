// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadOutWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Helldivers2Instance.h"
#include "Interface/CharacterHUDInterface.h"
#include "W_SettingStratagem.h"
#include "StratagemImgC.h"
#include "Stratagem/StratagemData.h"
#include "Characters/Player/DiversPlayerController.h"

void ULoadOutWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NextSetBtnN = 0;

	WBP_SettingStratagem = Cast<UW_SettingStratagem>(GetWidgetFromName(TEXT("WBP_SettingStratagem")));
	ensure(WBP_SettingStratagem);
	if (WBP_SettingStratagem)
	{
		WBP_SettingStratagem->SetLoadWidget(this);

		WBP_SettingStratagem->SetVisibility(ESlateVisibility::Hidden);
	}

	ensure(Btn_StratagemSet1);
	ensure(Btn_StratagemSet2);
	ensure(Btn_StratagemSet3);
	ensure(Btn_StratagemSet4);

	ensure(Btn_Img1);
	ensure(Btn_Img2);
	ensure(Btn_Img3);
	ensure(Btn_Img4);

	ensure(Btn_Ready);
	ensure(SettingAnimation);
	ensure(SpreadAnimation);
	ensure(FoldAnimation);

	BtnImgs.Add(Btn_Img1);
	BtnImgs.Add(Btn_Img2);
	BtnImgs.Add(Btn_Img3);
	BtnImgs.Add(Btn_Img4);
	for (auto Imgs : BtnImgs) {
		Imgs->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Btn_StratagemSet1) Btn_StratagemSet1->OnPressed.AddDynamic(this, &ULoadOutWidget::BtnFunc);
	if (Btn_StratagemSet2) Btn_StratagemSet2->OnPressed.AddDynamic(this, &ULoadOutWidget::BtnFunc);
	if (Btn_StratagemSet3) Btn_StratagemSet3->OnPressed.AddDynamic(this, &ULoadOutWidget::BtnFunc);
	if (Btn_StratagemSet4) Btn_StratagemSet4->OnPressed.AddDynamic(this, &ULoadOutWidget::BtnFunc);

	if (Btn_Ready) Btn_Ready->OnClicked.AddDynamic(this, &ULoadOutWidget::OnGameStart);

	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

	FWidgetAnimationDynamicEvent Tes;
	Tes.BindUFunction(this, FName("SettingAnimationPlayableToggle"));
	BindToAnimationStarted(SettingAnimation, Tes);

	bSettingClosed = true;

	FLinearColor ReadyBtnReadyColor = FLinearColor(0.973445f, 0.789702f, 0.035490f, 1);
	ButtonReadyStyle = Btn_Ready->GetStyle();
	ButtonReadyStyle.Normal.TintColor = FSlateColor(ReadyBtnReadyColor);
	ButtonReadyStyle.Hovered.TintColor = FSlateColor(ReadyBtnReadyColor);
	ButtonReadyStyle.Pressed.TintColor = FSlateColor(ReadyBtnReadyColor);

	ReadyBtnReadyColor.A = 0;
	ButtonInitStyle = Btn_Ready->GetStyle();
	ButtonInitStyle.Normal.TintColor = FSlateColor(ReadyBtnReadyColor);
	ButtonInitStyle.Hovered.TintColor = FSlateColor(ReadyBtnReadyColor);
	ButtonInitStyle.Pressed.TintColor = FSlateColor(ReadyBtnReadyColor);

	ReadyTextInitColor = FLinearColor(1.0, 0.792157f, 0.0, 1.f);
	BlackColor = FLinearColor(0.f, 0.f, 0.f, 1.f);

	BtnBackgroundColor = BtnBackground->GetColorAndOpacity();
}

void ULoadOutWidget::OnGameStart()
{
	bStart = !bStart;

	OnClickedStartBtn.ExecuteIfBound(bStart);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (bStart)
	{
		Btn_Ready->SetStyle(ButtonReadyStyle);

		BtnBackgroundColor.A = 0;
		BtnBackground->SetColorAndOpacity(BtnBackgroundColor);

		Text_StartCondition->SetColorAndOpacity(BlackColor);
		Text_StartCondition->SetText(FText::FromString(TEXT("준비 상태 전환")));

		TimerManager.SetTimer(StartDelay, [this]() {
			UUMGSequencePlayer* SequencePlayer = PlayAnimation(FoldAnimation);
			SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& SeqPlayer) { GameStart(); });
				}, 3.0f, false);
	}
	else
	{
		Btn_Ready->SetStyle(ButtonInitStyle);

		BtnBackgroundColor.A = 0.4f;
		BtnBackground->SetColorAndOpacity(BtnBackgroundColor);

		Text_StartCondition->SetColorAndOpacity(ReadyTextInitColor);
		Text_StartCondition->SetText(FText::FromString(TEXT("준비")));

		//TimerManager.ClearTimer(StartDelay);
	}
}

void ULoadOutWidget::GameStart()
{
	UHelldivers2Instance* GameInst = Cast<UHelldivers2Instance>(GetGameInstance());
	if (GameInst) {
		for (auto Img : BtnImgs) {
			UStratagemData* Data = Img->GetStratagemData();
			if (Data) GameInst->SetTempStratagemD(Data);
		}
	}

	GameInst->LoadBattleFieldLevel();
}

void ULoadOutWidget::BtnFunc()
{
	if (Btn_StratagemSet1->IsPressed()) ClickedBtnN = 0;
	else if (Btn_StratagemSet2->IsPressed()) ClickedBtnN = 1;
	else if (Btn_StratagemSet3->IsPressed()) ClickedBtnN = 2;
	else if (Btn_StratagemSet4->IsPressed()) ClickedBtnN = 3;

	PlayAnimation(SettingAnimation);

	OnStratagemSettingStart.Execute();
}

void ULoadOutWidget::SettingAnimationPlayableToggle()
{
	bSettingClosed = !bSettingClosed;
}

void ULoadOutWidget::SetStratagemData(class UButton* StratagemBtn)
{
	UWidget* ChildW = StratagemBtn->GetChildAt(0);
	UStratagemImgC* NewImg = Cast<UStratagemImgC>(ChildW);
	if (!NewImg) return;

	if (NextSetBtnN <= 3) {
		BtnImgs[NextSetBtnN]->SetBrushResourceObject(NewImg->GetBrush().GetResourceObject());
		BtnImgs[NextSetBtnN]->SetStratagemData(NewImg->GetStratagemData());
		BtnImgs[NextSetBtnN++]->SetVisibility(ESlateVisibility::Visible);

		BroughtBtn.Add(StratagemBtn);

		StratagemBtn->SetIsEnabled(false);
	}
	else {
		BtnImgs[ClickedBtnN]->SetBrushResourceObject(NewImg->GetBrush().GetResourceObject());
		BtnImgs[ClickedBtnN]->SetStratagemData(NewImg->GetStratagemData());

		BroughtBtn[ClickedBtnN]->SetIsEnabled(true);
		BroughtBtn[ClickedBtnN] = StratagemBtn;

		StratagemBtn->SetIsEnabled(false);
	}

	if (NextSetBtnN == 4)
	{
		ExitSettingStratagem();
	}
}

void ULoadOutWidget::VisibleWidget(bool bShow)
{
	if (bShow)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(SpreadAnimation);
	}
	else
	{
		UUMGSequencePlayer* SequencePlayer = PlayAnimation(FoldAnimation);
		SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& SeqPlayer)
			{
				SetVisibility(ESlateVisibility::Hidden);
			});
	}
}

void ULoadOutWidget::ExitSettingStratagem()
{
	if (!bSettingClosed)
	{
		PlayAnimation(SettingAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);

		OnStratagemSettingEnd.Execute();
	}
}