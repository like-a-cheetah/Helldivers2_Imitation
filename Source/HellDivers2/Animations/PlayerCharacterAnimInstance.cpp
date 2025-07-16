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

		if (PlayerAnimInterface)
		{
			ItemType = PlayerAnimInterface->GetCurrentItemType();
			CameraType = PlayerAnimInterface->GetCurrentPose();
			bRightButton = PlayerAnimInterface->IsRightButton();
			bLeftButton = PlayerAnimInterface->IsLeftButton();
			SuccedStratagem = PlayerAnimInterface->IsSucceededStratagem();
			bPullingPin = PlayerAnimInterface->IsPullingPin();
			bActiveConsole = PlayerAnimInterface->IsConsoleActive();
			bActiveRegIK = PlayerAnimInterface->IsConsoleActive();
			InputVector = PlayerAnimInterface->GetInputVector();
			ChestRot = PlayerAnimInterface->GetChestRot();
		}

		if(!Movement->IsFalling() && bActiveRegIK) CalculateFootGoalTransform(DeltaSeconds);
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

void UPlayerCharacterAnimInstance::CalculateFootGoalTransform(float DeltaTime)
{
	USkeletalMeshComponent* Mesh = Owner->GetMesh();

	float FootZ = Mesh->GetSocketLocation(TEXT("root_Socket")).Z;

	auto FootTrace = [this, DeltaTime, Mesh, FootZ](FName FootSocketName, float& FootGoalZ, FRotator& FootGoalRot) {
		FTransform FootTransform = Owner->GetMesh()->GetBoneTransform(FootSocketName, RTS_World);
		
		FVector LFootLoc = FootTransform.GetLocation();
		LFootLoc.Z = FootZ + 30.0f;

		FVector FootGoalTraceLoc = FootTransform.GetLocation();
		FootGoalTraceLoc.Z = FootZ - 50.0f;	//기존 발의 z값보다 50 아래까지 추적

		FCollisionShape Sphere = FCollisionShape::MakeSphere(20.0f);
		FHitResult HitResult;
		bool bHit = GetWorld()->SweepSingleByObjectType(
			HitResult,
			LFootLoc,
			FootGoalTraceLoc,
			FQuat::Identity,
			ECC_WorldStatic,
			Sphere
		);

		if (bHit)
		{
			float CalculatedZ = HitResult.ImpactPoint.Z - FootZ;
			
			FootGoalZ = FMath::FInterpTo(FootGoalZ, CalculatedZ, DeltaTime, 15.0f);

			FVector GroundNormal = Owner->GetActorTransform().InverseTransformVectorNoScale(HitResult.Normal);
			//FVector GroundNormal = HitResult.Normal;
			float PitchAngleRad = FMath::RadiansToDegrees(FMath::Atan2(GroundNormal.Y, GroundNormal.Z));
			float RollAngleRad = FMath::RadiansToDegrees(FMath::Atan2(GroundNormal.X, GroundNormal.Z));
			FRotator CalculatedRot = FRotator(PitchAngleRad, 0.0f, -RollAngleRad);

			FootGoalRot = FMath::RInterpTo(FootGoalRot, CalculatedRot, DeltaTime, 15.f);
		}
		else
		{
			FootGoalZ = 0;

			FootGoalRot = FRotator::ZeroRotator;
		}
	};

	FName FootSocketName = TEXT("l_foot");
	if (Mesh->DoesSocketExist(FootSocketName))
		FootTrace(FootSocketName, LFootGoalZ, LFootGoalRot);

	FootSocketName = TEXT("r_foot");
	if (Mesh->DoesSocketExist(FootSocketName))
		FootTrace(FootSocketName, RFootGoalZ, RFootGoalRot);

	//{
	//	FTransform FootTransform = Owner->GetMesh()->GetBoneTransform(FootSocketName, RTS_World);
	//
	//	FVector LFootLoc = Tr_LFoot.GetLocation();
	//	LFootLoc.Z = FootZ + 30.0f;
	//
	//	FVector FootGoalTraceLoc = Tr_LFoot.GetLocation();
	//	FootGoalTraceLoc.Z = FootZ - 50.0f;
	//
	//	FHitResult HitResult;
	//	bool bHit = GetWorld()->SweepSingleByObjectType(
	//		HitResult, 
	//		LFootLoc,
	//		FootGoalTraceLoc,
	//		FQuat::Identity, 
	//		ECC_WorldStatic, 
	//		Sphere
	//		);
	//	DrawDebugCylinder(GetWorld(), LFootLoc, FootGoalTraceLoc, 25.0f, 4, FColor::Blue, false, 0.1f);
	//
	//	float CalculatedZ = HitResult.ImpactPoint.Z - FootZ;
	//
	//	if (bHit) LFootGoalZ = FMath::FInterpTo(LFootGoalZ, CalculatedZ, DeltaTime, 15.0f);
	//	else LFootGoalZ = 0;
	//}
	//
	//{
	//	FTransform Tr_RFoot = Mesh->GetBoneTransform(TEXT("r_foot"), RTS_World);
	//
	//	FVector RFootLoc = Tr_RFoot.GetLocation();
	//	RFootLoc.Z = FootZ + 30.0f;
	//
	//	FVector FootGoalTraceLoc = Tr_RFoot.GetLocation();
	//	FootGoalTraceLoc.Z = FootZ - 50.0f;
	//
	//	FHitResult HitResult;
	//	bool bHit = GetWorld()->SweepSingleByObjectType(
	//		HitResult, 
	//		RFootLoc,
	//		FootGoalTraceLoc,
	//		FQuat::Identity, 
	//		ECC_WorldStatic, 
	//		Sphere
	//		);
	//	DrawDebugCylinder(GetWorld(), RFootLoc, FootGoalTraceLoc, 25.0f, 4, FColor::Blue, false, 0.1f);
	//
	//	float CalculatedZ = HitResult.ImpactPoint.Z - FootZ;
	//
	//	if (bHit) RFootGoalZ = FMath::FInterpTo(RFootGoalZ, CalculatedZ, DeltaTime, 15.0f);
	//	else RFootGoalZ = 0;
	//}
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