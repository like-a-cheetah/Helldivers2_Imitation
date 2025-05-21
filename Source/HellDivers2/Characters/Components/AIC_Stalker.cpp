// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIC_Stalker.h"

AAIC_Stalker::AAIC_Stalker()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HellDivers2/Characters/Enemys/Stalker/BB_Stalker.BB_Stalker'"));
    if (BBAssetRef.Object)
    {
        BBAsset = BBAssetRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HellDivers2/Characters/Enemys/Stalker/BT_Stalker.BT_Stalker'"));
    if (BTAssetRef.Object)
    {
        BTAsset = BTAssetRef.Object;
    }
}

//void AAIC_Stalker::SetTarget(AActor* Actor, FAIStimulus Stimulus)
//{
//    Super::SetTarget(Actor, Stimulus);
//}
