// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/ObjectInterface.h"

#include "BridgeHellpod.generated.h"

UCLASS()
class HELLDIVERS2_API ABridgeHellpod : public AActor, public IObjectInterface
{
	GENERATED_BODY()
	
public:	
	ABridgeHellpod();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> AS_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_HellpodReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_LockHellpod;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCol;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

protected:
	// IObjectInterface을(를) 통해 상속됨
	void Interact(AActor* Player) override;
};
