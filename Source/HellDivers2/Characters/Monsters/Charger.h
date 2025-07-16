// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Monsters/Enemy.h"
#include "Charger.generated.h"

/**
 * 
 */

UCLASS()
class HELLDIVERS2_API ACharger : public AEnemy
{
	GENERATED_BODY()
	
public:
	ACharger();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnOverlapBodySlam(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Rush();
	void ArmorBreak();

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_StartRush;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> BodySlamCollider;

	uint8 bEndRush : 1;

	uint8 bHeading: 1;

public:
	FORCEINLINE void SetbEndRush(bool InbEndRush) { bEndRush = InbEndRush; }
	FORCEINLINE bool IsEndRush() { return bEndRush; }

	FORCEINLINE void SetbHeading(bool InbHeading) { bHeading = InbHeading; }
	FORCEINLINE bool CanHeading() { return bHeading; }

};
