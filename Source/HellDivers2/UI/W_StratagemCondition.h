// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Stratagem/StratagemData.h"

#include "W_StratagemCondition.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UW_StratagemCondition : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UW_StratagemCondition(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//FORCEINLINE void SetData(UStratagemData* InData) { Data = DuplicateObject<UStratagemData>(InData, this); }
	void SetData(UStratagemData* InData);

	void ShowWidget(bool bShow);
	void SetInactiveW(bool bActive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnimation), Transient)
	TObjectPtr<class UWidgetAnimation> ShowAnim;	//이거 바인딩 안돼서 임시로 퍼블릭, BP에서 바인딩시킴

private:
	UPROPERTY()
	TObjectPtr<UStratagemData> Data;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Name;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> MacroBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> InactiveW;

	UPROPERTY()
	TArray<TObjectPtr<class UTexture2D>> Arrow;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> Macro;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UTextBlock> ConditionText;

	FColor CorrectColor;

	void SetMacro();
	void SetArrowColor(int n);
	void InitAllArrowColor();
	void SetCoolTimeText(FString Text);
	void SetHideMacroBox(bool bHide);

	float StratagemAnimPlayTime;
};
