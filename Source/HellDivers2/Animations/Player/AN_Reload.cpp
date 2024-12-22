// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AN_Reload.h"

#include "Interface/GunInterface.h"

void UAN_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IGunInterface *ItemInterface = Cast<IGunInterface>(MeshComp->GetOwner());
	if (ItemInterface)
	{
		ItemInterface->Reload();
	}
}
