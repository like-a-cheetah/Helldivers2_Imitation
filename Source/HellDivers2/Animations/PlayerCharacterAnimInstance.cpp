// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerCharacterAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "HellDivers2/Items/Item.h"
#include "Interface/PlayerAnimInterface.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		PlayerAnimInterface = Cast<IPlayerAnimInterface>(Owner);
	}

	RecoilTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f));
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		Pitch = Owner->GetControlRotation().Pitch;
		PronePitch = Owner->GetControlRotation().Pitch;
		if (PronePitch > 90.0f)
		{
			PronePitch = 0.0f;
		}
		Yaw = Owner->GetControlRotation().Yaw - Owner->GetActorRotation().Yaw;
		if (Yaw > 180)
		{
			Yaw -= 360.0f;
		}

		MovementYaw = CalculateMovementYaw();

		ItemType = PlayerAnimInterface->GetCurrentItemType();
		CameraType = PlayerAnimInterface->GetCurrentPose();
		bRightButton = PlayerAnimInterface->IsRightButton();
		SuccedStratagem = PlayerAnimInterface->IsSucceededStratagem();
		bPullingPin = PlayerAnimInterface->IsPullingPin();
	}

	FRotator CalRecoilRot = FMath::RInterpTo(Recoil.Rotator(), RecoilTransform.Rotator(), GetWorld()->GetDeltaSeconds(), 25.0f);
	FVector CalRecoilLoc = FMath::VInterpTo(Recoil.GetLocation(), RecoilTransform.GetLocation(), GetWorld()->GetDeltaSeconds(), 25.0f);
	Recoil.SetLocation(CalRecoilLoc);
	RecoilTransform.SetRotation(CalRecoilRot.Quaternion());
	
	FRotator CalRecoilTransformRot = FMath::RInterpTo(RecoilTransform.Rotator(), FRotator(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 25.0f);
	FVector CalRecoilTransformLoc = FMath::VInterpTo(RecoilTransform.GetLocation(), FVector(0.0f, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 25.0f);
	RecoilTransform.SetLocation(CalRecoilTransformLoc);
	RecoilTransform.SetRotation(CalRecoilTransformRot.Quaternion());
}

float UPlayerCharacterAnimInstance::CalculateMovementYaw()
{
	float CurrentYaw = Owner->GetActorRotation().Yaw;

	FVector MovementDirection = Owner->GetVelocity().GetSafeNormal();
	float TargetYaw = MovementDirection.Rotation().Yaw;

	float DeltaYaw = TargetYaw - CurrentYaw;

	if (DeltaYaw > 180.0f)
		DeltaYaw -= 360.0f;
	else if (DeltaYaw < -180.f)
		DeltaYaw += 360.0f;

	return DeltaYaw;
}

void UPlayerCharacterAnimInstance::ProceduralRecoil(float Multipler)
{
	FRotator RecoilRotation;
	FVector RecoilLocation;

	float Val1 = Multipler * FMath::FRandRange(-2.5f, -5.0f);
	float Val2 = Multipler * FMath::FRandRange(-0.8f, 0.8f);
	float Val3 = Multipler * FMath::FRandRange(-1.6f, 1.6f);
	RecoilRotation = FRotator(Val1, Val2, Val3);

	Val1 = Multipler * FMath::FRandRange(-0.16f, 0.16f);
	Val2 = Multipler * FMath::FRandRange(-2.1f, -1.1f);
	Val3 = Multipler * FMath::FRandRange(0.0f, 0.0f);
	RecoilLocation = FVector(Val1, Val2, Val3);

	RecoilTransform = FTransform(RecoilRotation, RecoilLocation);
	//RecoilTransform.SetRotation(RecoilRotation.Quaternion());
	//RecoilTransform.SetLocation(RecoilLocation);
}