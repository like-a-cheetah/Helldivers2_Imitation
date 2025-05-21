// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Monster/AN_CallTeam.h"

#include "Characters/Components/AIController_Enemy.h"

void UAN_CallTeam::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    APawn* OwnerPawn = Cast<APawn>(Owner);
    if (!OwnerPawn) return;

    AAIController_Enemy* Controller = Cast<AAIController_Enemy>(OwnerPawn->GetController());
    if (!Controller) return;

    Controller->CallTeam();
}