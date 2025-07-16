// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MissionTable.h"

#include "Components/WidgetComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "MovieSceneTrack.h"
#include "MovieSceneSection.h"

#include "GlobeComponent.h"
#include "Interface/PlayerControl.h"
#include "UI/W_MissionArea.h"
#include "Objects/BridgeHellpod.h"
#include "HellDivers2/Helldivers2Instance.h"
#include "UI/MissionAreaComponent.h"

AMissionTable::AMissionTable()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/STATIC_MESH/SM_Bridge_NAV.SM_Bridge_NAV'"));
	if (MeshRef.Object) StaticMeshComp->SetStaticMesh(MeshRef.Object);

	TiggerCollision->SetupAttachment(RootComponent);
	InformWidget->SetupAttachment(RootComponent);

	SkelMeshComp->DestroyComponent();

	MapWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("MapWidgetComp"));
	MapWidgetComp->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAssetRef(TEXT("/Game/HellDivers2/UI/MissionTable/WBP_MissionUI.WBP_MissionUI_C"));
	if (WidgetAssetRef.Succeeded()) MapWidgetComp->SetWidgetClass(WidgetAssetRef.Class);
	MapWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	MapWidgetComp->SetRelativeRotation(FRotator(90.000000, 180.000000, 180.000000));
	MapWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	MapWidgetComp->SetDrawSize(FVector2D(200.0f, 200.0f));
	//MapWidgetComp->SetWindowVisibility(EWindowVisibility::SelfHitTestInvisible);

	GlobeComp = CreateDefaultSubobject<UGlobeComponent>(TEXT("GlobeComp"));
	GlobeComp->SetupAttachment(RootComponent, TEXT("Socket_Globe"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GlobeMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HellDivers2/Objects/ShipInterior/Globe/Planet_SuperEarth_mesh_444.Planet_SuperEarth_mesh_444'"));
	if (GlobeMeshRef.Object) GlobeComp->SetStaticMesh(GlobeMeshRef.Object);

	EscapeType = EEscapeType::ESC;
}

void AMissionTable::BeginPlay()
{
	Super::BeginPlay();

	MapWidgetComp->SetWorldScale3D(FVector(0, 0, 0));


	FMovieSceneSequencePlaybackSettings PlaybackSet;
	PlaybackSet.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	PlaybackSet.bPauseAtEnd = true;

	ALevelSequenceActor* OutActor = nullptr;

	ULevelSequencePlayer* LSPlayer1 = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LS_ToSelectEarth,
		PlaybackSet,
		OutActor
	);

	CameraLSActor = OutActor;


	FMovieSceneSequencePlaybackSettings PlaybackSet2;
	PlaybackSet2.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;

	OutActor = nullptr;

	ULevelSequencePlayer* LSPlayer2 = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LS_GlobeZoom,
		PlaybackSet2,
		OutActor
	);

	GlobeLSActor = OutActor;
}

void AMissionTable::Interact_Implementation(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		User = Player;

		CameraLSActor->GetSequencePlayer()->Play();

		Player->SetFocusOnMissionTable(true, GlobeComp);

		if (!bSelectedArea)
		{
			GlobeLSActor->GetSequencePlayer()->Play();
		}

		bChangeArea = false;

		UW_MissionArea::OnSelectedMission.AddWeakLambda(this, [this, Actor](UWidgetComponent* WidgetComp) {
			bChangeArea = true;

			Escape_Implementation(Actor);
			});
	}
}

void AMissionTable::Escape_Implementation(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		CameraLSActor->GetSequencePlayer()->PlayReverse();

		if (bChangeArea)
		{
			if (!bSelectedArea) ABridgeHellpod::OnSetupBridgeHellpods.Broadcast(true);
		}
		else
		{
			GlobeLSActor->GetSequencePlayer()->PlayReverse();

			if (bSelectedArea) ABridgeHellpod::OnSetupBridgeHellpods.Broadcast(false);

			GlobeComp->GlobeHide();
		}

		bSelectedArea = bChangeArea;

		Player->SetFocusOnMissionTable(false, GlobeComp);

		SetActiveOverlapEvent(true);
		
		User = nullptr;
	}
}

void AMissionTable::SetupHellpodBridge()
{
}