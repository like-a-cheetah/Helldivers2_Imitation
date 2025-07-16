// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"

#include "Interface/ObjectInterface.h"

#include "MissionTable.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AMissionTable : public AInteractObj, public IObjectInterface
{
	GENERATED_BODY()
	
public:
	AMissionTable();

	void BeginPlay() override;

protected:
	// IObjectInterface을(를) 통해 상속됨
	void Interact_Implementation(AActor* Actor) override;

	void Escape_Implementation(AActor* Actor) override;

	void SetupHellpodBridge();

private:
	TObjectPtr<class IPlayerControl> User;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UWidgetComponent> MapWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UGlobeComponent> GlobeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class ULevelSequence> LS_GlobeZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class ULevelSequence> LS_ToSelectEarth;

	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> CameraLSActor;
	UPROPERTY()
	TObjectPtr<class ALevelSequenceActor> GlobeLSActor;

	uint8 bChangeArea : 1;
	uint8 bSelectedArea : 1;
};
