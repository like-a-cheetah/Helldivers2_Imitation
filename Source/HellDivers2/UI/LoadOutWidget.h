// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadOutWidget.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnStratagemSettingStart);
DECLARE_DELEGATE(FOnStratagemSettingEnd);

DECLARE_DELEGATE_OneParam(FOnClickedStartBtn, bool /*bStart*/);

UCLASS()
class HELLDIVERS2_API ULoadOutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
    UPROPERTY()
    TObjectPtr<class UW_SettingStratagem> WBP_SettingStratagem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_StratagemSet1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_StratagemSet2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_StratagemSet3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_StratagemSet4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UStratagemImgC> Btn_Img1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UStratagemImgC> Btn_Img2;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UStratagemImgC> Btn_Img3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UStratagemImgC> Btn_Img4;

    TArray<TObjectPtr<class UStratagemImgC>> BtnImgs;
    TArray<TObjectPtr<class UButton>> BroughtBtn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_Ready;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UTextBlock> Text_StartCondition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UImage> BtnBackground;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> SettingAnimation;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> SpreadAnimation;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> FoldAnimation;

private:
    uint8 ClickedBtnN;
    uint8 NextSetBtnN;

    uint8 bSettingClosed : 1;

    uint8 bStart : 1;

    FTimerHandle StartDelay;

    UFUNCTION()
    void OnGameStart();

    void GameStart();

    UFUNCTION()
    void BtnFunc();

    UFUNCTION()
    void SettingAnimationPlayableToggle();

private:
    FButtonStyle ButtonInitStyle;
    FButtonStyle ButtonReadyStyle;

    FSlateColor ReadyTextInitColor;
    FSlateColor BlackColor;

    FLinearColor BtnBackgroundColor;

public:
    UFUNCTION()
    void ExitSettingStratagem();

    void SetStratagemData(class UButton* StratagemBtn);

    UFUNCTION()
    void VisibleWidget(bool bShow);

    FOnStratagemSettingStart OnStratagemSettingStart;
    FOnStratagemSettingEnd OnStratagemSettingEnd;

    FOnClickedStartBtn OnClickedStartBtn;
};
