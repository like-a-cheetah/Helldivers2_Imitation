// Fill out your copyright notice in the Description page of Project Settings.


#include "Helldivers2Instance.h"

#include "Kismet/GameplayStatics.h"

void UHelldivers2Instance::LoadBattleFieldLevel()
{ 
	UGameplayStatics::OpenLevel(GetWorld(), SelectedBattleFieldLevel);
}
