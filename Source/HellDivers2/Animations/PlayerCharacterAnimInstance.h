// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Items/EItemType.h"
#include "Characters/Player/EPose.h"

#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HELLDIVERS2_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	EPose CameraType;	//나중에 Enum타입 클래스 따로 만들어서 PlayerCharacter 헤더 추가 없애기

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsIdle : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovingThreshould;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 JumpingThreshould : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float PronePitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	uint8 bRightButton : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	uint8 bDived : 1;

	UFUNCTION(BlueprintCallable)
	float CalculateMovementYaw();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stratagem)
	uint8 SuccedStratagem : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stratagem)
	uint8 bPullingPin : 1;

	class IPlayerAnimInterface* PlayerAnimInterface;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Recoil, Meta = (AllowPrivateAccess = "true"))
	FTransform RecoilTransform;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Recoil, Meta = (AllowPrivateAccess = "true"))
	FTransform Recoil;

public:
	UFUNCTION()
	void ProceduralRecoil(float Multipler);
};
