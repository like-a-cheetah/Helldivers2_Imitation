// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/Stratagem.h"
#include "Hellpod.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FOnPlayerArrive);

UCLASS()
class HELLDIVERS2_API AHellpod : public AStratagem
{
	GENERATED_BODY()
	
public:
	AHellpod();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	virtual void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	virtual void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	virtual void SpawnAttachMachine();

	bool bDecisionLandingPoint;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_RecallPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USkeletalMeshComponent> HellpodMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StopCollider", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxCollider;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComp;
	UPROPERTY()
	TObjectPtr<USoundWave> SW_Falling;
	UPROPERTY()
	TObjectPtr<USoundWave> SW_Landing;
	UPROPERTY()
	TObjectPtr<USoundWave> SW_HatchOpen;
	UPROPERTY()
	TObjectPtr<UAnimSequence> StartAnim;
	UPROPERTY()
	TObjectPtr<UAnimMontage> MT_ReadyToRanding;

	void CheckToLanding();
};
