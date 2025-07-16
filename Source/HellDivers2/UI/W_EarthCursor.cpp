// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_EarthCursor.h"

#include "Components/Image.h"

void UW_EarthCursor::NativeConstruct()
{
	Super::NativeConstruct();

	CursorImg->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	CursorImg->SetRenderScale(FVector2D(1.8f, 1.8f));
}

void UW_EarthCursor::PlayAnim(bool bHovered)
{
	if (bHovered)
	{
		PlayAnimation(HoveredAnim);
	}
	else
	{
		if (IsAnimationPlaying(HoveredAnim))
		{
			StopAnimation(HoveredAnim);
		}
		CursorImg->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		CursorImg->SetRenderScale(FVector2D(1.8f, 1.8f));

		FTimerHandle Tmp;
		GetWorld()->GetTimerManager().SetTimer(Tmp, [this]() {
			CursorImg->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
			CursorImg->SetRenderScale(FVector2D(1.8f, 1.8f));
			},
			.01f,     // 1초 후
			false     // 반복 X (한 번만 실행)
		);
	}
}
