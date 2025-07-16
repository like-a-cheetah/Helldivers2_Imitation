// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AcidComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnAcidCoolTimeEnd, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLDIVERS2_API UAcidComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAcidComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpitAcid();
	FORCEINLINE void SetMax_AcidCoolTime(float InTime) { Max_AcidCoolTime = InTime; }

private:
	UPROPERTY()
	TSubclassOf<class AAcidBall> AcidBall;

	UPROPERTY()
	TObjectPtr<ACharacter> Parent;

	float Max_AcidCoolTime;
	float Current_AcidCoolTime;

	FActorSpawnParameters SpawnParam;

	FTimerHandle CoolTimer;

public:
	FOnAcidCoolTimeEnd OnAcidCoolTimeEnd;
};
