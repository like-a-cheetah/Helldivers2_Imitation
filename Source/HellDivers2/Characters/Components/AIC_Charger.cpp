// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIC_Charger.h"

AAIC_Charger::AAIC_Charger()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HellDivers2/Characters/Enemys/Charger/BB_Charger.BB_Charger'"));
    if (BBAssetRef.Object)
    {
        BBAsset = BBAssetRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HellDivers2/Characters/Enemys/Charger/BT_Charger.BT_Charger'"));
    if (BTAssetRef.Object)
    {
        BTAsset = BTAssetRef.Object;
    }
}

void AAIC_Charger::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float CurrentRushCool = Blackboard->GetValueAsFloat(BBKEY_CURRENT_RUSH_COOLTIME);
    if (CurrentRushCool > 0.0f)
    {
        Blackboard->SetValueAsFloat(BBKEY_CURRENT_RUSH_COOLTIME, CurrentRushCool - DeltaTime);
    }
}