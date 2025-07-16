// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Monster/AN_SetActiveRotate.h"

#include "AIController.h"

#include "Interface/AnimMovementInterface.h"
#include "Characters/Components/AIController_Enemy.h"

void UAN_SetActiveRotate::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor) return;

    APawn* Pawn = Cast<APawn>(OwnerActor);
    if (!Pawn) return;
    IAnimMovementInterface* EnemyAnimInterface = Cast<IAnimMovementInterface>(Pawn);
    if (EnemyAnimInterface)
    {
        EnemyAnimInterface->SetRefreshAnimRot(bRefreshAnimRotate);
        UE_LOG(LogTemp, Log, TEXT("%d Can Rot"), bRefreshAnimRotate);
    }

    AAIController* AIController = Cast<AAIController>(Pawn->GetController());
    if (!AIController) return;
    AAIController_Enemy* AIC_Enemy = Cast<AAIController_Enemy>(Pawn->GetController());
    if(AIC_Enemy) AIC_Enemy->SetSmoothRotate(bSmoothRotate);
}
