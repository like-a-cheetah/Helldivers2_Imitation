// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AcidBall.generated.h"

UCLASS()
class HELLDIVERS2_API AAcidBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AAcidBall();


private:
	UPROPERTY(EditAnywhere, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Particle;

	UFUNCTION()
	virtual void OnAcidBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
