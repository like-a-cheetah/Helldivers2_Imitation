// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"

#include "Interface/ObjectInterface.h"

#include "ExtractionConsole.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AExtractionConsole : public AInteractObj, public IObjectInterface
{
	GENERATED_BODY()
	
public:
	AExtractionConsole();

	void Interact(AActor* Player) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TArray<uint8> Macro;
};
