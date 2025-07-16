// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GlobeComponent.generated.h"


DECLARE_DELEGATE(FOnCursorInit);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLDIVERS2_API UGlobeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UGlobeComponent();

	void GlobeHide();
	void SetGlobeCursor(FVector InEarthCursorPos, FRotator InEarthCursorRot);

	FORCEINLINE FVector GetStartWorldLoc() { return StartLoc; };

private:
	UPROPERTY()
	TObjectPtr<class UWidgetComponent> HoveredWidget;

	FOnCursorInit OnCursorInit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, Meta = (AllowPrivateAccess))
	TObjectPtr<class UWidgetComponent> EarthCursorWidget;

	FVector EarthCursorPos;
	FRotator EarthCursorRot;

	uint8 bActive : 1;

	virtual void BeginPlay() override;

	FVector StartLoc;
};
