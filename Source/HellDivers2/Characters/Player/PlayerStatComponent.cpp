// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MaxHp = 100.f;
	Hp = MaxHp;
	bPlayerDead = false;
}

void UPlayerStatComponent::SetHp(float NewHp)
{
	Super::SetHp(NewHp);

	OnHpChanged.Broadcast(NewHp);
}

float UPlayerStatComponent::ApplyDamage(float InDamage)
{
	float ResultDamage = Super::ApplyDamage(InDamage);

	OnHpChanged.Broadcast(Hp);


	if (bPlayerDead)
	{
		OnHpZero.Broadcast();
		
	}

	return ResultDamage;
}

