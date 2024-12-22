// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "StratagemImgC.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UStratagemImgC : public UImage
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stratagem")
	TObjectPtr<class UStratagemData> SData;

public:
	FORCEINLINE class UStratagemData* GetStratagemData() { return SData; }
	FORCEINLINE void SetStratagemData(class UStratagemData* InSData) { SData = InSData; }
};
