// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIC_Hunter.h"

AAIC_Hunter::AAIC_Hunter()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HellDivers2/Characters/Enemys/Hunter/BB_Hunter.BB_Hunter'"));
    if (BBAssetRef.Object)
    {
        BBAsset = BBAssetRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HellDivers2/Characters/Enemys/Hunter/BT_Hunter.BT_Hunter'"));
    if (BTAssetRef.Object)
    {
        BTAsset = BTAssetRef.Object;
    }
}

