// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BugHole.h"

#include "MonsterManager.generated.h"

UCLASS()
class HELLDIVERS2_API AMonsterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterManager();

protected:
	virtual void BeginPlay() override;

public:	

	UFUNCTION() void EraseMonster(AActor* Actor) { UE_LOG(LogTemp, Log, TEXT("Monster Manage --")); EnemyNum--; }
	UFUNCTION() void EraseMonsterHole(AActor* BrokenHole) { UE_LOG(LogTemp, Log, TEXT("Hole Manage --")); Holes.Remove(BrokenHole); }

private:
	UPROPERTY(VisibleAnywhere)
	uint32 EnemyNum;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Holes;
};
