// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_StratagemCondition.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

#include "Interface/CharacterHUDInterface.h"

UW_StratagemCondition::UW_StratagemCondition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef1(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Left.Left'"));
	if (TextureRef1.Succeeded()) Arrow.Add(TextureRef1.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef2(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Right.Right'"));
	if (TextureRef2.Succeeded()) Arrow.Add(TextureRef2.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef3(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Down.Down'"));
	if (TextureRef3.Succeeded()) Arrow.Add(TextureRef3.Object);
	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureRef4(TEXT("/Script/Engine.Texture2D'/Game/HellDivers2/UI/Source/Stratagem/Up.Up'"));
	if (TextureRef4.Succeeded()) Arrow.Add(TextureRef4.Object);

	CorrectColor = FColor::FromHex(TEXT("050505CF"));
}

void UW_StratagemCondition::NativeConstruct()
{
	Super::NativeConstruct();

	if (Data)
	{
		if(Icon) Icon->Brush.SetResourceObject(Data->Texture);

		if (Name)
		{
			if (Data->Name.IsNone())
				Name->SetText(FText::FromName(Data->Texture.GetFName()));
			else
				Name->SetText(FText::FromName(Data->Name));
		}

		if(MacroBox) SetMacro();
	}

	ICharacterHUDInterface* HUDPawn = Cast<ICharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetStratagemConditionWidget(this);
	}
}

void UW_StratagemCondition::SetData(UStratagemData* InData)
{
	Data = InData;
	Data->OnActiveWidget.BindUObject(this, &UW_StratagemCondition::SetInactiveW);
	Data->OnCorrectMacro.BindUObject(this, &UW_StratagemCondition::SetArrowColor);
	Data->OnSetCoolTimeText.BindUObject(this, &UW_StratagemCondition::SetCoolTimeText);
	Data->OnHideMacroBox.BindUObject(this, &UW_StratagemCondition::SetHideMacroBox);
	Data->ShowConditionWidgetDelegate.BindUObject(this, &UW_StratagemCondition::ShowWidget);
}

void UW_StratagemCondition::ShowWidget(bool bShow)
{
	if (bShow)
	{
		InactiveW->SetVisibility(ESlateVisibility::Hidden);
		PlayAnimation(ShowAnim);
	}
	else
	{
		PlayAnimation(ShowAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
	}
}

void UW_StratagemCondition::SetInactiveW(bool bActive)
{
	if (bActive)
	{
		//UE_LOG(LogTemp, Log, TEXT("Blind hid"));
		InactiveW->SetVisibility(ESlateVisibility::Hidden);
		InitAllArrowColor();
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Blind vis"));
		InactiveW->SetVisibility(ESlateVisibility::Visible);
	}
}

void UW_StratagemCondition::SetMacro()
{
	for (int8 ArrowVal : Data->Macro)
	{
		ArrowVal -= 1;
		UTexture* Texture = Arrow[ArrowVal];
		UImage* ArrowImg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (ArrowImg)
		{
			ArrowImg->Brush.SetResourceObject(Texture);
			MacroBox->AddChild(ArrowImg);
		}
	}
}
void UW_StratagemCondition::SetArrowColor(int n)
{
	UImage* Img = Cast<UImage>(MacroBox->GetChildAt(n));
	if (Img)
	{
		Img->SetColorAndOpacity(CorrectColor);
	}
}

void UW_StratagemCondition::InitAllArrowColor()
{
	for (int i = 0; i < Data->Macro.Num(); i++)
	{
		UImage* Img = Cast<UImage>(MacroBox->GetChildAt(i));
		if (Img)
		{
			Img->SetColorAndOpacity(FLinearColor::White);
		}
	}
}

void UW_StratagemCondition::SetCoolTimeText(FString Text)
{
	ConditionText->SetText(FText::FromString(Text));
}

void UW_StratagemCondition::SetHideMacroBox(bool bHide)
{
	if (bHide)
	{
		MacroBox->SetVisibility(ESlateVisibility::Hidden);
		ConditionText->SetVisibility(ESlateVisibility::Visible);
		ConditionText->SetText(FText::FromString(FString::Printf(TEXT("준비"))));
	}
	else
	{
		MacroBox->SetVisibility(ESlateVisibility::Visible);
		ConditionText->SetVisibility(ESlateVisibility::Hidden);
	}

}
