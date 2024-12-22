// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Player/AnimNotifyChangeItem.h"

#include "Interface/GunInterface.h"

void UAnimNotifyChangeItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		Cast<IGunInterface>(MeshComp->GetOwner())->Reload();
	}
}
