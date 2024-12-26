// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadOutWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"

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
	ensure(StartAnimation);

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

	if (Btn_Ready) Btn_Ready->OnClicked.AddDynamic(this, &ULoadOutWidget::GameStart);

	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void ULoadOutWidget::GameStart()
{
	UHelldivers2Instance* GInst = Cast<UHelldivers2Instance>(GetGameInstance());
	if (GInst) {
		for (auto Img : BtnImgs) {
			UStratagemData* Data = Img->GetStratagemData();
			if (Data) GInst->SetTempStratagemD(Data);
		}
	}

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(StartAnimation);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& SeqPlayer)
		{
			ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
			if (HUDPawn) HUDPawn->LoadLevel(FName(TEXT("InGameTestmap")));
		});
}

void ULoadOutWidget::BtnFunc()
{
	if (Btn_StratagemSet1->IsPressed()) ClickedBtnN = 0;
	else if (Btn_StratagemSet2->IsPressed()) ClickedBtnN = 1;
	else if (Btn_StratagemSet3->IsPressed()) ClickedBtnN = 2;
	else if (Btn_StratagemSet4->IsPressed()) ClickedBtnN = 3;

	PlayAnimation(SettingAnimation);
}

void ULoadOutWidget::SetStratagemData(class UButton* StratagemBtn)
{
	UWidget* ChildW = StratagemBtn->GetChildAt(0);
	UStratagemImgC* NewImg = Cast<UStratagemImgC>(ChildW);
	if (!NewImg) return;

	if (NextSetBtnN <= 3) {
		BtnImgs[NextSetBtnN]->SetBrushResourceObject(NewImg->Brush.GetResourceObject());
		BtnImgs[NextSetBtnN]->SetStratagemData(NewImg->GetStratagemData());
		BtnImgs[NextSetBtnN++]->SetVisibility(ESlateVisibility::Visible);

		BroughtBtn.Add(StratagemBtn);

		StratagemBtn->SetIsEnabled(false);
	}
	else {
		BtnImgs[ClickedBtnN]->SetBrushResourceObject(NewImg->Brush.GetResourceObject());
		BtnImgs[ClickedBtnN]->SetStratagemData(NewImg->GetStratagemData());

		BroughtBtn[ClickedBtnN]->SetIsEnabled(true);
		BroughtBtn[ClickedBtnN] = StratagemBtn;

		StratagemBtn->SetIsEnabled(false);
	}

	if(NextSetBtnN == 4) PlayAnimation(SettingAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
}

void ULoadOutWidget::VisibleWidget(bool bShow)
{
	if (bShow) SetVisibility(ESlateVisibility::Visible);
	else SetVisibility(ESlateVisibility::Hidden);
}

void ULoadOutWidget::ExitSettingStratagem()
{
	PlayAnimation(SettingAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
}