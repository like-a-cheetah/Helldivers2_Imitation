// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/ExtractionConsole.h"

#include "Interface/PlayerControl.h"

AExtractionConsole::AExtractionConsole()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef (TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Props/GAMEPLAY/CONSOLE_2/SK_CONSOLE_02.SK_CONSOLE_02'"));
	if (MeshRef.Object) SkelMeshComp->SetSkeletalMesh(MeshRef.Object);
}

void AExtractionConsole::Interact(AActor* Player)
{
	IPlayerControl* Player = Cast<IPlayerControl>(Actor);
	if (Player)
	{
		Player->EnterHellpodBridge(this);
	}
}
