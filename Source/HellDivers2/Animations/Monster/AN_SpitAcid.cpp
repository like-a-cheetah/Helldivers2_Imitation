// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Monster/AN_SpitAcid.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Interface/AcidCompInterface.h"
#include "Characters/Components/AcidComponent.h"

void UAN_SpitAcid::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAcidCompInterface* Enemy = Cast<IAcidCompInterface>(MeshComp->GetOwner());
	if (Enemy) Enemy->GetAcidComp()->SpitAcid();
}
