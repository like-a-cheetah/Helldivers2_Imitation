// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/StratagemData.h"

void UStratagemData::SetbActive(bool InbActive)
{
	bActive = InbActive;
	OnActiveWidget.ExecuteIfBound(InbActive);
}

void UStratagemData::CorrectMacro(int n)
{
	OnCorrectMacro.ExecuteIfBound(n);
}

void UStratagemData::SetCoolTime(UWorld* World)
{
	CoolTime = MaxCoolTime;
	bCoolTime = true;

	FTimerHandle HideWidgetTimer;
	World->GetTimerManager().SetTimer(HideWidgetTimer, [this]()
	{ 
		if(ShowConditionWidgetDelegate.IsBound()) 
			ShowConditionWidgetDelegate.ExecuteIfBound(false);
	}, 2.0f, false);
}

