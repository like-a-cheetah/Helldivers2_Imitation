// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"
#include "HellpodAttachment.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSetup);

UCLASS()
class HELLDIVERS2_API AHellpodAttachment : public AInteractObj
{
	GENERATED_BODY()
	
public:
	void Setup();
	void Hide();

	UPROPERTY(BlueprintAssignable, Category = "Event", Meta = (AllowPrivateAccess = true))
	FOnEndSetup OnEndSetup;
private:

	UPROPERTY(BlueprintReadWrite, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimSequence> AS_Setup;
	UPROPERTY(BlueprintReadWrite, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimSequence> AS_Hide;
};
