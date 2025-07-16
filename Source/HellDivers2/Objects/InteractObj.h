// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"

#include "InteractObj.generated.h"

UENUM(BlueprintType)
enum class EEscapeType: uint8
{
	None = 0,
	ESC,
	E
};

UCLASS()
class HELLDIVERS2_API AInteractObj : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractObj();

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, Category = Collision)
	//TObjectPtr<class UShapeComponent> TiggerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision)
	TObjectPtr<class USphereComponent> TiggerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicInfo)
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicInfo)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = Widget)
	TObjectPtr<class UWidgetComponent> InformWidget;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Widget/*, Meta = (AllowprivateAccess = "true")*/)
	FText Description;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> Mat_OutLiner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ObjType)
	EEscapeType EscapeType;

public:
	virtual void SetActiveOverlapEvent(bool bActive);

	FORCEINLINE EEscapeType GetEscapeType() { return EscapeType; }
};
