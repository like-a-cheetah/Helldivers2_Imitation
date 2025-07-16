// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionTableUI.h"

#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Helldivers2Instance.h"
#include "Characters/Player/PlayerCharacter.h"

#include "Helldviers2ModeBase.h"

void UMissionTableUI::NativeConstruct()
{
    Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Visible);
    Map->SetVisibility(ESlateVisibility::Visible);
    CursorImg->SetVisibility(ESlateVisibility::Hidden);

    Map->OnMouseButtonDownEvent.BindUFunction(this, TEXT("FunctionName"));
}

FEventReply UMissionTableUI::FunctionName(FGeometry InGeometry, const FPointerEvent& InMouseEvent)
{
    MouseEvent = InMouseEvent;

    if (InMouseEvent.IsTouchEvent() || InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FVector2D LocalClickPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        HandleMapClick(LocalClickPos, InGeometry.GetLocalSize());

        SetCursorPos(LocalClickPos);

        Player->OnPlayStartGameSequencer.Execute(false);

        bOnMouseButtonDownEvent = true;

        return FEventReply(false);
    }

    return FEventReply();
}

FReply UMissionTableUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FEventReply Reply;
    Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    return Reply.NativeReply;
}

void UMissionTableUI::SetActiveCursor(bool bActive)
{
    if (bActive) CursorImg->SetVisibility(ESlateVisibility::HitTestInvisible);
    else CursorImg->SetVisibility(ESlateVisibility::Hidden);
}

void UMissionTableUI::SetCursorPos(FVector2D Pos)
{
    SetActiveCursor(true);
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CursorImg->Slot))
    {
        FVector2D Size = CanvasSlot->GetSize();
        FVector2D AdjustedPos = FVector2D(Pos.X - Size.X * 0.5f, Pos.Y - Size.Y * 0.5f);
        LastHoverPos = Pos;

        CanvasSlot->SetPosition(AdjustedPos);
    }
}

void UMissionTableUI::CustomClickEvent()
{
    if (bOnMouseButtonDownEvent)
    {
        //Map->OnMouseButtonDownEvent.Execute(GetCachedGeometry(), MouseEvent);

        FVector2D LocalClickPos = LastHoverPos;
        HandleMapClick(LocalClickPos, GetCachedGeometry().GetLocalSize());

        SetCursorPos(LocalClickPos);

        Player->OnPlayStartGameSequencer.Execute(false);
    }
}

//FReply UMissionTableUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//    if (InMouseEvent.IsTouchEvent() || InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
//    {
//        // 터치 위치 처리 함수 호출
//        FVector2D LocalClickPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
//        HandleMapClick(LocalClickPos, InGeometry.GetLocalSize());
//
//        return FReply::Handled();
//    }
//
//    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
//}

void UMissionTableUI::HandleMapClick(const FVector2D& LocalClickPos, const FVector2D& WidgetSize)
{
    float X = LocalClickPos.Y;
    float Y = LocalClickPos.X;

    float V = (WidgetSize.X - X) / (WidgetSize.X - 20.f);
    float U = Y / (WidgetSize.Y - 20.f);

    float RealX = FMath::Lerp(-28400, 34700, V);
    float RealY = FMath::Lerp(-34600, 34700, U);

    FVector SpawnLocation = FVector(RealX, RealY, 0.0f);
    
    UHelldivers2Instance* GInst = Cast<UHelldivers2Instance>(GetGameInstance());
    if(GInst) {
        GInst->SetStartPoin(SpawnLocation);
    }
}
