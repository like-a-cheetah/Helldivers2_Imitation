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
}

void UPlayerStatComponent::SetHp(float NewHp)
{
	Super::SetHp(NewHp);

	OnHpChanged.Broadcast(NewHp);
}

float UPlayerStatComponent::ApplyDamage(float InDamage)
{
	float ResultDamage = Super::ApplyDamage(InDamage);
	/*if (ResultDamage == -1.0f) return ResultDamage;

	OnHpChanged.Broadcast(Hp);*/

	return ResultDamage;
}

