// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/FireSubsystem.h"

#include "Fire.h"
#include "Characters/Components/CharacterStatComponent.h"

void UFireSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FString LevelName = GetWorld()->GetMapName();
	if (LevelName == "InGameTestmap") bTickable = true;


	OverlappedCharacters.Empty();
}

void UFireSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bTickable) return;

	for (auto Character = OverlappedCharacters.CreateIterator(); Character; ++Character)
	{
		if (Character.Value().Key <= 0)
			Character.RemoveCurrent();
		else
		{
			if (Character.Key() && Character.Value().Value <= 0.0f)
			{
				Character.Key()->ApplyDamage(5.0f * DeltaTime);
	
				Character.Value().Value = MaxDamageCooldownTime;
			}
			else
				Character.Value().Value -= DeltaTime;
		}
	}
}

TStatId UFireSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFireWorldSubsystem, STATGROUP_Tickables);
}