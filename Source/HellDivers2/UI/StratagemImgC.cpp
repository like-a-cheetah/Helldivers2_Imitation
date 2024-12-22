// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StratagemImgC.h"

#include "Stratagem/StratagemData.h"

void UStratagemImgC::SetImage()
{
	Brush.SetResourceObject(SData->Texture);
	Brush.SetImageSize(FVector2D(65.0f, 65.0f));
}
