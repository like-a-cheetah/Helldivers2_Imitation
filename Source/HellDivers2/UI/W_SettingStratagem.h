// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SettingStratagem.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UW_SettingStratagem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<class UButton>> Btns;

	UPROPERTY()
	TObjectPtr<class ULoadOutWidget> LoadOutWidget;

	TObjectPtr<class UButton> SelectedBtnN;

	UFUNCTION()
	void SetStratagem();

	UFUNCTION()
	void SetBtnN();
	
	void FindAllStratagemBtns(UWidget* ParentWidget);

public:
	void SetLoadWidget(class ULoadOutWidget* In_LoadOutWidget);
};
