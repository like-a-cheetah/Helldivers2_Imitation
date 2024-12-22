// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_StratagemNotice.h"

#include "Components/VerticalBox.h"

#include "W_StratagemCondition.h"



UW_StratagemNotice::UW_StratagemNotice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UW_StratagemCondition> W_StratagemConditionCRef(TEXT("/Game/HellDivers2/UI/InGame/WBP_StratagemCondition.WBP_StratagemCondition_C"));
	if (W_StratagemConditionCRef.Class) ConditionWidgetC = W_StratagemConditionCRef.Class;
}

void UW_StratagemNotice::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(Box);
}

void UW_StratagemNotice::SetStratagemWidget(TArray<UStratagemData*> Datas)
{
	for (auto* Data : Datas) {
		UW_StratagemCondition* StratagemCondition = CreateWidget<UW_StratagemCondition>(this, ConditionWidgetC);
		StratagemCondition->SetData(Data);
		
		Box->AddChild(StratagemCondition);

		W_Stratagems.Add(StratagemCondition);
	}
}
