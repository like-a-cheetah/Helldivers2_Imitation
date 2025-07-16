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

	virtual void BeginPlay() override;

protected:
	void Interact_Implementation(AActor* Actor) override;
	void Escape_Implementation(AActor* Actor) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowprivateAccess = true))
	TSubclassOf<AActor> Hellpod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowprivateAccess = true));
	TObjectPtr<class UMacroWidgetComponent> MacroWidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "Spawn")
	TObjectPtr<USphereComponent> SpawnPoint;

	UFUNCTION()
	void SucceessMacro();
};
