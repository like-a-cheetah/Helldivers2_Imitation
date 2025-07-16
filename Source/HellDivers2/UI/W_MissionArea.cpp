// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_MissionArea.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "HellDivers2/Helldivers2Instance.h"

FOnSelectedMission UW_MissionArea::OnSelectedMission;
FOnHoveredMission UW_MissionArea::OnHoveredMission;

void UW_MissionArea::NativeConstruct()
{
	Super::NativeConstruct();

	LvBtn->OnClicked.AddDynamic(this, &UW_MissionArea::OnButtonClicked);
	LvBtn->OnHovered.AddDynamic(this, &UW_MissionArea::OnButtonHovered);
	LvBtn->OnUnhovered.AddDynamic(this, &UW_MissionArea::OnButtonUnhovered);
}

void UW_MissionArea::OnButtonClicked()
{
	UGameInstance* GInst = GetGameInstance();
	if (UHelldivers2Instance* CurGInst = Cast<UHelldivers2Instance>(GInst))
	{
		CurGInst->SetBattleFieldLevel(LvName);
	}
	
	OnSelectedMission.Broadcast(WidgetComp);
}

void UW_MissionArea::OnButtonHovered()
{
	OnHoveredMission.Broadcast(WidgetComp);
}

void UW_MissionArea::OnButtonUnhovered()
{
	OnHoveredMission.Broadcast(nullptr);
}
