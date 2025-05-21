// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Orbit/Orbit_Walking.h"

void AOrbit_Walking::SetStrikePoint()
{
	Super::SetStrikePoint();

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * (10000.0f / (Times * MaxBombN)));
}
