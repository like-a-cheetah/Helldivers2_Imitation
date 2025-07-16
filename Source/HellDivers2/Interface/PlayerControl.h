// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputMappingContext.h"
#include "PlayerControl.generated.h"

UINTERFACE(BlueprintType)
class UPlayerControl : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnBulletEnemyHit);
DECLARE_DELEGATE_RetVal_OneParam(uint8, FOnInputMacro, uint8 /*Macro*/);


class HELLDIVERS2_API IPlayerControl
{
	GENERATED_BODY()

protected:
	TObjectPtr<class ALevelSequenceActor> CurrentLVSequenceActor;

public:
	virtual void Recoil(float Pitch, float Yaw) = 0;
	virtual void GetCurrentZ() = 0;
	virtual void Summoned() = 0;
	virtual void SetNearbyInteractable(AActor* Object) = 0;
	virtual void Heal() = 0;

	virtual void VaultPostProcess() = 0;
	
	//각 오브젝트의 Interact에 따라 동작할 함수
	virtual void EnterHellpodBridge(AActor* BridgeHellpod) = 0;
	virtual void LeaveHellpodBridge(AActor* BridgeHellpod) = 0;
	virtual void SetInteractConsole(bool bInteract, UMeshComponent* ConsoleWidget) = 0;
	virtual void SetFocusOnMissionTable(bool bInteract, class UGlobeComponent* GlobeComponent) = 0;

	virtual float RidePelican(FTransform SitTr) = 0;

	FOnBulletEnemyHit OnBulletEnemyHit;
	FOnInputMacro OnInputMacro;

	virtual void SetCurrentLVSequenceActor(class ALevelSequenceActor* InLvSequenceActor) = 0;
};
