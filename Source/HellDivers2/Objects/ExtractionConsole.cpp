// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ExtractionConsole.h"\

#include "UI/MacroWidgetComponent.h"
#include "Interface/PlayerControl.h"

AExtractionConsole::AExtractionConsole()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef (TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Props/GAMEPLAY/CONSOLE_2/SK_CONSOLE_02.SK_CONSOLE_02'"));
	if (MeshRef.Object) SkelMeshComp->SetSkeletalMesh(MeshRef.Object);

	MacroWidgetComp = CreateDefaultSubobject<UMacroWidgetComponent>(TEXT("WidgetComp"));
	MacroWidgetComp->SetupAttachment(RootComponent, TEXT("porter_bone_10"));
	MacroWidgetComp->SetRelativeRotation(FRotator(0.f, -90.0f, 0.0f));
	MacroWidgetComp->OnSuccessMacro.AddDynamic(this, &AExtractionConsole::SucceessMacro);

	SpawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetSphereRadius(16.f);
	SpawnPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	SpawnPoint->SetHiddenInGame(true);

	EscapeType = EEscapeType::E;
}

void AExtractionConsole::BeginPlay()
{
	Super::BeginPlay();
}

void AExtractionConsole::Interact_Implementation(AActor* Actor)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		MacroWidgetComp->Interact(Player);
	}
}

void AExtractionConsole::SucceessMacro()
{
	SetActiveOverlapEvent(false);

	FTransform SpawnTr = SpawnPoint->GetComponentTransform();
	FVector Loc = SpawnTr.GetLocation();
	Loc.Z += 80000.0f;
	SpawnTr.SetLocation(Loc);
	GetWorld()->SpawnActor<AActor>(Hellpod, SpawnTr);
}

void AExtractionConsole::Escape_Implementation(AActor* Actor)
{
	MacroWidgetComp->Escape();
}
