// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadOutWidget.generated.h"

/**
 * 
 */
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UButton> Btn_Ready;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> SettingAnimation;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> StartAnimation;

private:
    uint8 ClickedBtnN;
    uint8 NextSetBtnN;

    UFUNCTION()
    void GameStart();

    UFUNCTION()
    void BtnFunc();

public:
    UFUNCTION()
    void ExitSettingStratagem();

    void SetStratagemData(UWidget* StratagemImg);
};
