// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/HellpodAttachment.h"

void AHellpodAttachment::Setup()
{
	SkelMeshComp->PlayAnimation(AS_Setup, false);

	float Delay = AS_Setup->GetPlayLength();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, ([this]() { 
		OnEndSetup.Broadcast(); UE_LOG(LogTemp, Log, TEXT("fds"));
		}), Delay, false);
}

void AHellpodAttachment::Hide()
{
	SkelMeshComp->PlayAnimation(AS_Hide, false);
}
