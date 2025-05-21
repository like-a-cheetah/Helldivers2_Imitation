// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInst_Charger.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/Monsters/Charger.h"

void UAnimInst_Charger::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Owner)
	{
		Charger = Cast<ACharger>(Enemy);
	}
}

void UAnimInst_Charger::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Charger)
	{
		bEndRush = Charger->IsEndRush();
		bHeading = Charger->CanHeading();
	}
}