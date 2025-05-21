// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FireSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UFireSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Tick(float DeltaTime) override;

	TStatId GetStatId() const;

private:
	TMap<class UCharacterStatComponent*, TPair<int32, float>> OverlappedCharacters;

	float MaxDamageCooldownTime;

	uint8 bTickable : 1;

public:
	FORCEINLINE void RegisterOverlappedCharacter(UCharacterStatComponent* Stat) {
		if (OverlappedCharacters.Find(Stat))
			OverlappedCharacters[Stat].Key++;
		else
			OverlappedCharacters.Add(Stat, TPair<int32, float>(1, 0.f));
	}

	FORCEINLINE void OverlapEndCharacter(UCharacterStatComponent* Stat) {
		if (OverlappedCharacters.Find(Stat))
		{
			if (--OverlappedCharacters[Stat].Key <= 0)
				OverlappedCharacters.Remove(Stat);
		}
	}
};
