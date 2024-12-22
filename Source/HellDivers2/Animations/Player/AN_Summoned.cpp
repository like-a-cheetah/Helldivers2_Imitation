// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AN_Summoned.h"

#include "Interface/PlayerControl.h"

void UAN_Summoned::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IPlayerControl* Player = Cast<IPlayerControl>(MeshComp->GetOwner());
	if (Player)
	{
		Player->Summoned();
	}
}
