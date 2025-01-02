// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/StratagemData.h"

void UStratagemData::SetbActive(bool InbActive)
{
	bActive = InbActive;
	OnActiveWidget.Execute(InbActive);
}

void UStratagemData::CorrectMacro(int n)
{
	OnCorrectMacro.Execute(n);
}

void UStratagemData::SetCoolTime(UWorld* World)
{
	CoolTime = MaxCoolTime;
	bCoolTime = true;

	FTimerHandle HideWidgetTimer;
	World->GetTimerManager().SetTimer(HideWidgetTimer, [this]()
	{ 
		if(ShowConditionWidgetDelegate.IsBound()) 
			ShowConditionWidgetDelegate.Execute(false); 
	}, 2.0f, false);
}

