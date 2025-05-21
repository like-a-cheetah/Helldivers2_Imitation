// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BugHole.generated.h"

DECLARE_DELEGATE_OneParam(FOnBugHoleBeginPlay, AActor*);

UCLASS()
class HELLDIVERS2_API ABugHole : public AActor
{
	GENERATED_BODY()
	
public:	
	ABugHole();

	virtual void BeginPlay() override;

public:
	static FOnBugHoleBeginPlay OnBugHoleBeginPlay;

private:
	uint8 bOnceBeginPlayEvent : 1;

	uint8 bBroken : 1;

	uint32 MonsterN;

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> SM_Hole;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UGeometryCollectionComponent> GCC_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMesh> SM_BrokenHole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	TSubclassOf<class AEnemy> Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USphereComponent> SpawnSphere;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	TObjectPtr<class USphereComponent> SpawnPatrolPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MaxSpawnTime;
	float SpawnTime;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnMonster();

	UFUNCTION()
	virtual void HoleBroken();
};
