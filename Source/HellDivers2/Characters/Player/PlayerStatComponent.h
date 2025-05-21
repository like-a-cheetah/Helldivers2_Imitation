// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/Components/CharacterStatComponent.h"

#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrenadeNChangedDelegate, int /*Grenade*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemChangedDelegate, int /*GrenadeN*/, int /*MagazineN*/, int /*SyringeN*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLDIVERS2_API UPlayerStatComponent : public UCharacterStatComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

protected:
	virtual void InitializeComponent() override;

public:	
	FOnHpChangedDelegate OnHpChanged;
	FOnItemChangedDelegate OnItemChanged;
	FOnGrenadeNChangedDelegate OnGrenadeChanged;

	virtual void SetHp(float NewHp) override;
	virtual float ApplyDamage(float InDamage) override;
};
