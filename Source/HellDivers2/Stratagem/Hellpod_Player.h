// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/HellPod.h"
#include "Hellpod_Player.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHellpod_Player : public AHellpod
{
	GENERATED_BODY()

public:
	AHellpod_Player();

	virtual void BeginPlay() override;

protected:
	virtual void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	virtual void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	virtual void SpawnAttachMachine() override;

private:
	FOnPlayerArrive OnPlayerArrive;

	float InputX;
	float InputY;

	FVector ForwardVec;
	FVector RightVec;

	TObjectPtr<AActor> RidingPlayer;

public:
	void AttchPlayer(class APlayerCharacter* Player);

private:
	UFUNCTION()
	void MoveX(float Value);
	UFUNCTION()
	void MoveY(float Value);
};
