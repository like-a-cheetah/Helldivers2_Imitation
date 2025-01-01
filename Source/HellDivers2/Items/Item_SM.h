// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Item_SM.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AItem_SM : public AItem
{
	GENERATED_BODY()
	
public:
	AItem_SM();

	virtual void SetBaseData() override;
};
