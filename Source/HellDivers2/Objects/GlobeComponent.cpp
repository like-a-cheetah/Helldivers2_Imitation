// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/GlobeComponent.h"

#include "Components/WidgetComponent.h"

#include "UI/W_MissionArea.h"
#include "UI/MissionAreaComponent.h"
#include "UI/W_EarthCursor.h"


UGlobeComponent::UGlobeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	EarthCursorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("EarthCursorWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> EarthCursorWidgetRef(TEXT("/Game/HellDivers2/UI/MissionTable/WBP_GlobeCursor.WBP_GlobeCursor_C"));
	if (EarthCursorWidgetRef.Succeeded())
	{
		EarthCursorWidget->SetWidgetClass(EarthCursorWidgetRef.Class);
		EarthCursorWidget->SetDrawSize(FVector2D(1000, 1000));
		EarthCursorWidget->SetRelativeRotation(FRotator(-180.0f, 0.0f, 0.0f));
		EarthCursorWidget->SetWidgetSpace(EWidgetSpace::World);
		EarthCursorWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EarthCursorWidget->SetGenerateOverlapEvents(false);
		EarthCursorWidget->SetHiddenInGame(true);
		EarthCursorWidget->SetBlendMode(EWidgetBlendMode::Transparent);
	}
}

void UGlobeComponent::GlobeHide()
{
	EarthCursorWidget->SetHiddenInGame(true);
}

void UGlobeComponent::SetGlobeCursor(FVector InEarthCursorPos, FRotator InEarthCursorRot)
{
	bActive = true;

	if (!HoveredWidget)
	{
		if (EarthCursorWidget->GetAttachParent())
			OnCursorInit.Execute();

		EarthCursorPos = InEarthCursorPos;
		EarthCursorRot = InEarthCursorRot;

		EarthCursorWidget->SetWorldLocation(EarthCursorPos);
		EarthCursorWidget->SetWorldRotation(EarthCursorRot);
	}

	EarthCursorWidget->SetHiddenInGame(false);
}

void UGlobeComponent::BeginPlay()
{
	Super::BeginPlay();

	StartLoc = GetComponentLocation();

	EarthCursorWidget->InitWidget();

	UW_EarthCursor* W_Cursor =  Cast<UW_EarthCursor>(EarthCursorWidget->GetWidget());

	OnCursorInit.BindLambda([W_Cursor, this]() {
		EarthCursorWidget->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		W_Cursor->PlayAnim(false);
		});
	
	UW_MissionArea::OnHoveredMission.AddWeakLambda(this, [this, W_Cursor](UWidgetComponent* InHoveredWidget) {
			HoveredWidget = InHoveredWidget;

			if (!bActive)
				return;

			W_Cursor->PlayAnim(InHoveredWidget != nullptr);

			if (InHoveredWidget)
			{
				EarthCursorWidget->AttachToComponent(InHoveredWidget, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
			else
			{
				EarthCursorWidget->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

				EarthCursorWidget->SetWorldLocation(EarthCursorPos);
				EarthCursorWidget->SetWorldRotation(EarthCursorRot);
			}
		});

	UW_MissionArea::OnSelectedMission.AddWeakLambda(this, [this](UWidgetComponent* HoveredWidget) {
		if (HoveredWidget)
		{
			bActive = false;
			EarthCursorWidget->AttachToComponent(HoveredWidget, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		});

	TArray<USceneComponent*> MissionAreaComponents = GetAttachChildren();
	for (auto Component : MissionAreaComponents)
	{
		if (UMissionAreaComponent* MissonAreaComponent = Cast<UMissionAreaComponent>(Component))
		{
			MissonAreaComponent->SetInitTransform();
		}
	}

	SetRelativeLocation(FVector(0.0f, 0.0f, -162.0f));
	SetWorldScale3D(FVector::ZeroVector);
}