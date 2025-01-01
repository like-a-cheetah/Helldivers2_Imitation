// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Items/EItemType.h"

#include "ItemData.generated.h"

/**
 * 
 */

UCLASS()
class HELLDIVERS2_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UItemData();

public:
	UPROPERTY(EditAnywhere, Category = BasicInfo)
	FName Name;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	TObjectPtr<class USkeletalMesh> SkelMesh;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	TObjectPtr<class UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = BasicInfo);
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	uint32 Count;

	UPROPERTY(EditAnywhere, Category = BasicInfo)	//데미지, 힐 등 체력에 영향을 끼치는 값
	float Val;

	//UPROPERTY(EditAnywhere, Category = BasicInfo)
	//TMap<"", UAnimMontage> Montages;
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> TakeOutMontage;

	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> InsertMontage;
};
