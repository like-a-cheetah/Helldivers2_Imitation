// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_StratagemNotice.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UW_StratagemNotice : public UUserWidget
{
	GENERATED_BODY()

public:
	UW_StratagemNotice(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TSubclassOf<class UW_StratagemCondition> ConditionWidgetC;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> Box;

	UPROPERTY()
	TArray<TObjectPtr<class UW_StratagemCondition>> W_Stratagems;

public:
	void SetStratagemWidget(TArray<class UStratagemData*> Datas);	
};
