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

