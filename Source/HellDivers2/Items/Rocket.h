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

protected:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float SplashRadius;

private:
	void BeginPlay();

	virtual void HitPostProcess(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit) override;

	UFUNCTION()
	virtual void OnOverlapBegin_Splash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
