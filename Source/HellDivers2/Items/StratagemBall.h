// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "StratagemBall.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AStratagemBall : public AItem
{
	GENERATED_BODY()

public:
	AStratagemBall();

	void BeginPlay();

public:
	FORCEINLINE void SetStratagem(TSubclassOf<AActor> InStratagemClass, uint8 InStratagemType) { StratagemClass = InStratagemClass; StratagemType = InStratagemType; }

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> NC_Laser;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> NS_Blue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> NS_Red;

	UPROPERTY()
	TObjectPtr<class USoundWave> SW_BallLoop;
	UPROPERTY()
	TObjectPtr<class UAnimSequence> AS_Lockup;

	UPROPERTY()
	TSubclassOf<AActor> StratagemClass;

	UFUNCTION()
	void SpawnStratagem();

	uint8 StratagemType;
};
