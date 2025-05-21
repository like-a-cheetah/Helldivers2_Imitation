// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIC_Warrior.h"

AAIC_Warrior::AAIC_Warrior()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HellDivers2/Characters/Enemys/Warrior/BB_Warrior.BB_Warrior'"));
    if (BBAssetRef.Object)
    {
        BBAsset = BBAssetRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HellDivers2/Characters/Enemys/Warrior/BT_Warrior.BT_Warrior'"));
    if (BTAssetRef.Object)
    {
        BTAsset = BTAssetRef.Object;
    }
}
