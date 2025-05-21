// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLDIVERS2_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	float Hp;

public:	
	void SetMaxHp(float NewMaxHP);
	virtual void SetHp(float NewHp);

	UFUNCTION(BlueprintPure, Category = "Stat")
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	
	UFUNCTION(BlueprintPure, Category = "Stat")
	FORCEINLINE float GetCurHp() { return Hp; }

	UFUNCTION(BlueprintPure, Category = "Stat")
	virtual float ApplyDamage(float InDamage);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHpZeroDelegate OnHpZero;
};
