// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ConsoleWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelSlot.h"

UConsoleWidget::UConsoleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef1(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Left.Left'"));
	if (TextureRef1.Succeeded()) ArrowSourceImages.Add(TextureRef1.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef2(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Right.Right'"));
	if (TextureRef2.Succeeded()) ArrowSourceImages.Add(TextureRef2.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef3(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Down.Down'"));
	if (TextureRef3.Succeeded()) ArrowSourceImages.Add(TextureRef3.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef4(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Up.Up'"));
	if (TextureRef4.Succeeded()) ArrowSourceImages.Add(TextureRef4.Object);

	CorrectColor = FColor::FromHex(TEXT("050505CF"));
}

void UConsoleWidget::NativeConstruct()
{
	for (int8 ArrowVal : Macro)
	{
		ArrowVal -= 1;
		UTexture* Texture = ArrowSourceImages[ArrowVal];
		UImage* ArrowImg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (ArrowImg)
		{
			ArrowImg->Brush.SetResourceObject(Texture);
			ArrowImg->Brush.ImageSize = FVector2D(30, 30);

			UHorizontalBoxSlot* HorizonSlot = MacroBox->AddChildToHorizontalBox(ArrowImg);
			HorizonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			HorizonSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			HorizonSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

			ArrowImages.Add(ArrowImg);
		}
	}
}

void UConsoleWidget::SetMacro(TArray<uint8> InMacro)
{
	Macro = InMacro;
}

void UConsoleWidget::SetArrowsCorrect(uint8 Index)
{
	ArrowImages[Index]->SetColorAndOpacity(CorrectColor);
}

void UConsoleWidget::InitAllArrowsColor()
{
	for(auto ArrowImag : ArrowImages) ArrowImag->SetColorAndOpacity(FLinearColor::White);
}
