// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AN_Reload.h"

#include "Characters/Player/PlayerCharacter.h"
#include "Interface/GunInterface.h"

void UAN_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Gun = MeshComp->GetOwner();
	APlayerCharacter* Player = Cast<APlayerCharacter>(Gun->GetOwner());
	if (Player)
	{
		Player->SuccessReload();
	}
}
