// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputMappingContext.h"
#include "PlayerControl.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerControl : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IPlayerControl
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Recoil(float Pitch, float Yaw) = 0;
	virtual void GetCurrentZ() = 0;
	virtual void Summoned() = 0;
	virtual void SetNearbyInteractable(AActor* Object) = 0;
	
	//각 오브젝트의 Interact에 따라 동작할 함수
	virtual void EnterHellpodBridge() = 0;
};
