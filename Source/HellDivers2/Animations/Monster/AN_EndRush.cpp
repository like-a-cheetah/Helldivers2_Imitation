// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Monster/AN_EndRush.h"

#include "Interface/AnimMovementInterface.h"

void UAN_EndRush::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	IAnimMovementInterface* Enemy = Cast<IAnimMovementInterface>(Owner);

	Enemy->SetStop();
	if (Enemy->OnAttackEnded.IsBound()) Enemy->OnAttackEnded.Execute();
}
