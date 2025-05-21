// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/PoseableMeshComponent.h"

#include "Ship.generated.h"

DECLARE_DELEGATE(FOnResetSpawnLocation);

UCLASS()
class HELLDIVERS2_API AShip : public AActor
{
	GENERATED_BODY()
	
public:	
	AShip();

	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp1;
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp2;
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp3;
	TObjectPtr<USkeletalMeshComponent> SkelMeshComp4;
	TObjectPtr<UPoseableMeshComponent> Turret_MeshComp1;
	TObjectPtr<UPoseableMeshComponent> Turret_MeshComp2;

public:	
	FORCEINLINE FVector GetSocketLocation(FName SocketName) {
		if (!SocketName.IsNone() && SkelMeshComp1->DoesSocketExist(SocketName))
			return SkelMeshComp1->GetSocketLocation(SocketName);
		
		return GetActorLocation();
	}

	FORCEINLINE FVector GetTurretSocketLocation(FName SocketName) {
		if (!SocketName.IsNone() && Turret_MeshComp1->DoesSocketExist(SocketName))
			return Turret_MeshComp1->GetSocketLocation(SocketName);

		return GetActorLocation();
	}

	//UFUNCTION(BlueprintImplementableEvent)
	void SetTurretTurn(FVector TargetLoc);
	FORCEINLINE void SetTurretSpin(bool bInSpin) {
		bSpin = bInSpin; 
		//SpinPitch = Turret_MeshComp1->GetBoneRotationByName(TEXT("Turret_Barrel"), EBoneSpaces::ComponentSpace).Pitch;
	}

	FOnResetSpawnLocation OnResetSpawnLoc;

private:
	FRotator TargetRot;

	uint8 bRotateTurret : 1;
	uint8 bRotateYaw : 1;
	uint8 bRotatePitch : 1;

	uint8 bSpin : 1;

	float SpinPitch;
};
