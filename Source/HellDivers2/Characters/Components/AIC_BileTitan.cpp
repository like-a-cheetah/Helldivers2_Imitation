// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIC_BileTitan.h"

AAIC_BileTitan::AAIC_BileTitan()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HellDivers2/Characters/Enemys/BileTitan/BB_BileTitan.BB_BileTitan'"));
    if (BBAssetRef.Object)
    {
        BBAsset = BBAssetRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HellDivers2/Characters/Enemys/BileTitan/BT_BileTitan.BT_BileTitan'"));
    if (BTAssetRef.Object)
    {
        BTAsset = BTAssetRef.Object;
    }
}

void AAIC_BileTitan::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DecreaseAcidCoolTime(DeltaTime);
}

void AAIC_BileTitan::DecreaseAcidCoolTime(float DeltaTime)
{
    float CurrentAcidCool = Blackboard->GetValueAsFloat(BBKEY_ACID_COOLTIME);

    if (CurrentAcidCool >= 0.0f)
    {
        Blackboard->SetValueAsFloat(BBKEY_ACID_COOLTIME, CurrentAcidCool - DeltaTime);
    }
}
