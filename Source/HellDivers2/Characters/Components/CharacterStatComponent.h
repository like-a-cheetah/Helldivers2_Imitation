// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLDIVERS2_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	float MaxHp;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	float Hp;

	uint8 bPlayerDead : 1;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetMaxHp(float NewMaxHP);
	void SetHp(float NewHp);

	UFUNCTION(BlueprintPure, Category = "Stat")
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	
	UFUNCTION(BlueprintPure, Category = "Stat")
	FORCEINLINE float GetCurHp() { return Hp; }

	UFUNCTION(BlueprintPure, Category = "Stat")
	virtual float ApplyDamage(float InDamage);
};
