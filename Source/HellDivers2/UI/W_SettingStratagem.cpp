// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_SettingStratagem.h"

#include "Components/Button.h"

#include "LoadOutWidget.h"
#include "StratagemImgC.h"

void UW_SettingStratagem::NativeConstruct()
{
	if (WidgetTree) {
		FindAllStratagemBtns(GetRootWidget());
	}
}

void UW_SettingStratagem::SetLoadWidget(ULoadOutWidget* In_LoadOutWidget)
{
    LoadOutWidget = In_LoadOutWidget;
}


void UW_SettingStratagem::SetStratagem()
{ 
    if (SelectedBtnN && SelectedBtnN->GetChildrenCount() > 0) {
        LoadOutWidget->SetStratagemData(SelectedBtnN);
    }
}

void UW_SettingStratagem::SetBtnN()
{
    for (auto Btn : Btns) {
        if (Btn->IsPressed()) SelectedBtnN = Btn;
    }
}

void UW_SettingStratagem::FindAllStratagemBtns(UWidget* ParentWidget)
{
    if (!ParentWidget) return;

    UButton* Btn = Cast<UButton>(ParentWidget);
    if (Btn)
    {
        Btn->OnClicked.AddDynamic(this, &UW_SettingStratagem::SetStratagem);
        Btn->OnPressed.AddDynamic(this, &UW_SettingStratagem::SetBtnN);
        
        UStratagemImgC* BtnImg = Cast<UStratagemImgC>(Btn->GetChildAt(0));
        if (BtnImg) BtnImg->SetImage();

        Btns.Add(Btn);
    }

    UPanelWidget* Panel = Cast<UPanelWidget>(ParentWidget);
    if (Panel)
    {
        for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
        {
            UWidget* Child = Panel->GetChildAt(i);
            FindAllStratagemBtns(Child);
        }
    }
}
