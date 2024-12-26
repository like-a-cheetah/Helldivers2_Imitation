// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DiversPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API ADiversPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiversPlayerController();

	void SetMouseCursor(bool bActive);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class ULoadOutWidget> LoadOutWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class ULoadOutWidget> LoadOutWidget;
};
