// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stratagem/HellPod.h"
#include "Hellpod_Supply.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API AHellpod_Supply : public AHellpod
{
	GENERATED_BODY()

public:
	AHellpod_Supply();
	virtual void BeginPlay() override;

private:
	void SpawnAttachMachine();
	void FoldMachine();

protected:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	//TObjectPtr<UAnimationAsset> DefaultAnim;	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	//TObjectPtr<UAnimationAsset> SpreadAnim;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	//TObjectPtr<UAnimationAsset> FoldAnim;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", Meta = (AllowPrivateAccess = "true"));
	//TObjectPtr<USkeletalMeshComponent> AttachMesh;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	//TSubclassOf<class AItem> SupplyItem;

	//TArray<TObjectPtr<AActor>> SupplyItems;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class AHellpodAttachment> HellpodAttachment;

	//virtual void SpawnSupplyItems();

};
