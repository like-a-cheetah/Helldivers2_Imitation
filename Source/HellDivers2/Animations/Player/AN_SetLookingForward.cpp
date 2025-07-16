// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AN_SetLookingForward.h"

#include "Characters/Player/PlayerCharacter.h"

void UAN_SetLookingForward::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Player) {
		Player->SetLookingForward(bActive);
	}
}