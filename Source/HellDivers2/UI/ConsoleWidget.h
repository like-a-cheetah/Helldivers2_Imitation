// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConsoleWidget.generated.h"

/**
 * 
 */

UCLASS()
class HELLDIVERS2_API UConsoleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UConsoleWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetMacro(TArray<uint8> InMacro);

	void SetArrowsCorrect(uint8 Index);
	void InitAllArrowsColor();

private:
	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> MacroBox;

	UPROPERTY()
	TArray<TObjectPtr<class UTexture2D>> ArrowSourceImages;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> ArrowImages;

	TArray<uint8> Macro;

	FColor CorrectColor;
};
