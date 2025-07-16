// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MissionArea.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedMission, class UWidgetComponent* /*WidgetComp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoveredMission, class UWidgetComponent* /*WidgetComp*/);

UCLASS()
class HELLDIVERS2_API UW_MissionArea : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	FORCEINLINE void SetWidgetComp(class UWidgetComponent* InWidgetComp) { WidgetComp = InWidgetComp; }

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

	static FOnSelectedMission OnSelectedMission;

	static FOnHoveredMission OnHoveredMission;

private:
	UPROPERTY()
	TObjectPtr<class UWidgetComponent> WidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FName LvName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true), meta = (BindWidget))
	TObjectPtr<class UButton> LvBtn;
};
