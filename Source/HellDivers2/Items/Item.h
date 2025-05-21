// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractObj.h"

#include "Data/ItemData.h"

#include "Item.generated.h"

DECLARE_DELEGATE_OneParam(FOnPickedUp, bool /*bPickedUp*/);

UCLASS()
class HELLDIVERS2_API AItem : public AInteractObj
{
	GENERATED_BODY()
	
public:	
	AItem();

	void BeginPlay();

	virtual void SetBaseData();

	FOnPickedUp OnPickedUp;

	void Throw(FVector Force);

protected:
	UPROPERTY(EditAnywhere, Category = BasicInfo)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	TObjectPtr<class UItemData> ItemData;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	FName Name;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	uint32 Count;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	float Val;

	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	//UFUNCTION()
	//void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	TObjectPtr<UAnimMontage> InsertMontage;

	UPROPERTY(EditAnywhere, Category = BasicInfo)
	TObjectPtr<UAnimMontage> TakeOutMontage;

public:
	FORCEINLINE EItemType GetItemType() { return ItemType; }
	FORCEINLINE UAnimMontage* GetInsertMontage() { return InsertMontage; }
	FORCEINLINE UAnimMontage* GetTakeOutMontage() { return TakeOutMontage; }
	FORCEINLINE FName GetSocketName() { return SocketName; }
	//FORCEINLINE USkeletalMeshComponent* GetSkelMeshComp() { return SkelMeshComp; }
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComp() { return StaticMeshComp; }
	FORCEINLINE FName GetName() { return Name; }
	FORCEINLINE int32 GetCount() { return Count; }
	FORCEINLINE void SetCount(int32 NewCount) { Count = NewCount; }
	FORCEINLINE void Consume() { Count--; }
};
