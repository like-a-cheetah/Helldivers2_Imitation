// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stratagem.generated.h"

UCLASS()
class HELLDIVERS2_API AStratagem : public AActor
{
	GENERATED_BODY()
	
public:	
	AStratagem();

protected:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh");
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp;
};
