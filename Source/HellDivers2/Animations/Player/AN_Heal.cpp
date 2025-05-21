// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AN_Heal.h"

#include "Interface/PlayerControl.h"


void UAN_Heal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IPlayerControl* Player = Cast<IPlayerControl>(MeshComp->GetOwner());
	if (Player)
	{
		Player->Heal();
	}
}
