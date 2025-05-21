// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Orbit/Orbit_Gatling.h"

void AOrbit_Gatling::BeginPlay()
{
	Super::BeginPlay();

	Ship->OnResetSpawnLoc.BindUObject(this, &AOrbit_Gatling::SetSpawnLoc);

	Ship->SetTurretTurn(GetActorLocation());
}

void AOrbit_Gatling::SetSpawnLoc()
{
	SpawnLoc = Ship->GetTurretSocketLocation(MuzzleSocketNames[0]);
}

void AOrbit_Gatling::OrbitStartAction()
{
	Super::OrbitStartAction();

	Ship->SetTurretSpin(true);
}

void AOrbit_Gatling::OrbitEndAction()
{
	Super::OrbitEndAction();

	Ship->SetTurretSpin(false);
}
