// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Stratagem.h"
#include "HellPodPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHellPodPlayer : public AStratagem
{
	GENERATED_BODY()
	
public:
	AHellPodPlayer();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;

private:
	TObjectPtr<UAnimSequence> StartAnim;
	TObjectPtr<UAnimMontage> MT_RecallPlayer;

	TObjectPtr<USkeletalMeshComponent> TestMesh;
};
