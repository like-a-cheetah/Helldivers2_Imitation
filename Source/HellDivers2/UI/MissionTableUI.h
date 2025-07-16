// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionTableUI.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UMissionTableUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	FEventReply FunctionName(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetActiveCursor(bool bActive);

	void SetCursorPos(FVector2D Pos);

	FORCEINLINE void SetInteractedPlayer(class APlayerCharacter* InteractedPlayer) { Player = InteractedPlayer; }

	void CustomClickEvent();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UImage> Map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	TObjectPtr<class UImage> CursorImg;

	TObjectPtr<class APlayerCharacter> Player;

	FVector2D LastHoverPos;

	uint8 bOnMouseButtonDownEvent : 1;

	FPointerEvent MouseEvent;

private:
	void HandleMapClick(const FVector2D& LocalClickPos, const FVector2D& WidgetSize);
};
