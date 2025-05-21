// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Monster/AN_EnableAttackBoneCollision.h"

#include "Characters/Monsters/Enemy.h"

void UAN_EnableAttackBoneCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (Enemy && !BoneName.IsNone())
	{
		Enemy->SetBoneColloisionActivity(BoneName, bActive);
	}
}
