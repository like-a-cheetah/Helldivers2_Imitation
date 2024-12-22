// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"



#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AGrenade : public AItem
{
	GENERATED_BODY()
	
public:
	AGrenade();

private:
	void Bomb();
	
	UFUNCTION()
	virtual void OnExplosionRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void PullingPin();

	void BeginPlay();

private:
	UPROPERTY(BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> VFX;

	float FuseTime;

	TArray<AActor*> overlapActors;

	TObjectPtr<USphereComponent> Splash;

};
