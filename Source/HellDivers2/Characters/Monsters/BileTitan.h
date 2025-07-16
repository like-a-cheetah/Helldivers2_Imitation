// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Monsters/Enemy.h"

#include "Interface/AcidCompInterface.h"

#include "BileTitan.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API ABileTitan : public AEnemy, public IAcidCompInterface
{
	GENERATED_BODY()
	
public:
	ABileTitan();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_SpitAcid;
	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_BasicAttack;
	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_Rampage;

public:
	void StartSpitAcid(FOnMontageEnded OnMontageEnd);

	void StartBasicAttack(FOnMontageEnded OnMontageEnd);
	void StartRampage(FOnMontageEnded OnMontageEnd);

protected:	
	virtual void OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void BeginActivity() override;

public:
	UAcidComponent* GetAcidComp() override;
};
