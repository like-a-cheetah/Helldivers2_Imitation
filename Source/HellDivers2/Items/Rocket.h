// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Bullet.h"
#include "Rocket.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API ARocket : public ABullet
{
	GENERATED_BODY()
	
public:
	ARocket();

private:
	void BeginPlay();

	UFUNCTION()
	virtual void OnOverlapBegin_Rocket(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapBegin_Splash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> BoomVFX;

	UPROPERTY(BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> Splash;
};
