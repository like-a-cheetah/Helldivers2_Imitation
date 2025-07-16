// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_Supply.h"

#include "Objects/HellpodAttachment.h"

AHellpod_Supply::AHellpod_Supply()
{
}

void AHellpod_Supply::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Childs;
	GetAttachedActors(Childs);

	for (AActor* Child : Childs)
	{
		if (AHellpodAttachment* Attachment = Cast<AHellpodAttachment>(Child))
		{
			HellpodAttachment = Attachment;
			break;
		}
	}
}

void AHellpod_Supply::SpawnAttachMachine()
{
	Super::SpawnAttachMachine();

	HellpodAttachment->Setup();
	//AttachMesh->SetCollisionProfileName(TEXT("BlockAll"));
	//AttachMesh->PlayAnimation(SpreadAnim, false);
}

void AHellpod_Supply::FoldMachine()
{
	HellpodAttachment->Hide();
}