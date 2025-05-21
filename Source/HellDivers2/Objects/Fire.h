// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fire.generated.h"

UCLASS()
class HELLDIVERS2_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	AFire();

protected:

public:	
	//virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnFireOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnFireOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OffFire();

private:
	TObjectPtr<class UCapsuleComponent> Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UParticleSystemComponent> PS_Fire;

	//static TMap<class UCharacterStatComponent*, TPair<int32, float>> OverlappedCharacters;
	TObjectPtr<class UFireSubsystem> FireSubsystem;
};
