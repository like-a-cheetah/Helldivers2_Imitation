// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"

#include "Interface/ObjectInterface.h"

#include "BridgeHellpod.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetupBridgeHellpods, bool /*bSetup*/);

UCLASS()
class HELLDIVERS2_API ABridgeHellpod : public AInteractObj, public IObjectInterface
{
	GENERATED_BODY()
	
public:	
	ABridgeHellpod();

	static FOnSetupBridgeHellpods OnSetupBridgeHellpods;

protected:
	virtual void BeginPlay() override;

private:

	TObjectPtr<AActor> User;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> AS_Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> AS_ReadyIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_Setup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_Reset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_HellpodReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_LockHellpod;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> MT_UnlockHellpod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class ULevelSequence> LS_ToStratagemSetting;

	TObjectPtr<class ALevelSequenceActor> LSActor;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UBoxComponent> BoxCol;

protected:
	// IObjectInterface을(를) 통해 상속됨
	void Interact_Implementation(AActor* Actor) override;

	void Escape_Implementation(AActor* Actor) override;

	virtual void SetActiveOverlapEvent(bool bActive) override;
	//void SetupHellpod(bool bActive);
};
