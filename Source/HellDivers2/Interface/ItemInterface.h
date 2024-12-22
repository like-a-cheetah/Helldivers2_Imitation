// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Items/EItemType.h"

#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLDIVERS2_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EItemType GetItemType() = 0;
	virtual UAnimMontage* GetInsertMontage() = 0;
	virtual UAnimMontage* GetTakeOutMontage() = 0;
	virtual FName GetSocketName() = 0;
	virtual class USkeletalMeshComponent* GetSkelMeshComp() = 0;
	virtual FName GetName() = 0;
	virtual int32 GetCount() = 0;
	virtual void SetCount(int32 NewCount) = 0;
	virtual void Consume() = 0;
};
