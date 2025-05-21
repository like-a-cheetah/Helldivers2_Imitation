// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MonsterManager.h"

#include "EngineUtils.h"

#include "Characters/Monsters/Enemy.h"

AMonsterManager::AMonsterManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMonsterManager::BeginPlay()
{
	//Super::BeginPlay();
	
    //Holes.Empty();

    //AEnemy::OnEnemyBeginPlay.BindLambda([this](APawn* Pawn) 
    //    { 
    //        EnemyNum++;
    //        Pawn->OnDestroyed.AddDynamic(this, &AMonsterManager::EraseMonster);
    //    });

    //ABugHole::OnBugHoleBeginPlay.BindLambda([this](AActor* Actor)
    //    {
    //        Holes.Add(Actor);
    //        Actor->OnDestroyed.AddDynamic(this, &AMonsterManager::EraseMonsterHole);
    //    });
}

