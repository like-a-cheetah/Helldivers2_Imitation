// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"
#include "Pelican.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API APelican : public AInteractObj
{
	GENERATED_BODY()
	
public:
	APelican();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;

private:
	TObjectPtr<UAnimSequence> AS_EndGame;

	uint8 bRotate : 1;
	float StartYaw;
	float TargetYaw;
};
