// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/DiversPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "UI/HUDWidget.h"
#include "UI/LoadOutWidget.h"

ADiversPlayerController::ADiversPlayerController()
{
	static ConstructorHelpers::FClassFinder<UHUDWidget> HUDWidgetRef(TEXT("/Game/HellDivers2/UI/InGame/WBP_PlayerHud.WBP_PlayerHud_C"));
	if (HUDWidgetRef.Class)
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<ULoadOutWidget> LoadOutWidgetRef(TEXT("/Game/HellDivers2/UI/LoadOut/WBP_LoadOut.WBP_LoadOut_C"));
	if (LoadOutWidgetRef.Class)
	{
		LoadOutWidgetClass = LoadOutWidgetRef.Class;
	}

	//CameraAnimModifier = CreateDefaultSubobject<UCameraAnimationCameraModifier>(TEXT("CameraAnimModifier"));
}

void ADiversPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName != "TestShip")
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget) HUDWidget->AddToViewport();
	}
	else
	{
		LoadOutWidget = CreateWidget<ULoadOutWidget>(this, LoadOutWidgetClass);
		if (LoadOutWidget)
		{
			LoadOutWidget->SetDesiredSizeInViewport(FVector2D(700.0f, 700.0f));
			LoadOutWidget->SetPositionInViewport(FVector2D(-90.0f, 200.0f));
			LoadOutWidget->AddToViewport();
		}
	}
}

void ADiversPlayerController::SetMouseCursor(bool bActive)
{
	bShowMouseCursor = bActive;

	if (bActive)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 고정 해제
		SetInputMode(InputMode);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}