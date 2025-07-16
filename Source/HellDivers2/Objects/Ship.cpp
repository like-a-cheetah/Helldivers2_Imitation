// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Ship.h"

#include "Kismet/KismetSystemLibrary.h"

AShip::AShip()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SkelMeshComp1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp1"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/SHIP_EXTERIOR/SKM_SHIP_EXTERIOR_1.SKM_SHIP_EXTERIOR_1'"));
	if (SkelMeshRef1.Succeeded()) SkelMeshComp1->SetSkeletalMeshAsset(SkelMeshRef1.Object);
	SkelMeshComp1->SetupAttachment(RootComponent);

	SkelMeshComp2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp2"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/SHIP_EXTERIOR/SKM_SHIP_EXTERIOR_2B.SKM_SHIP_EXTERIOR_2B'"));
	if (SkelMeshRef2.Succeeded()) SkelMeshComp2->SetSkeletalMeshAsset(SkelMeshRef2.Object);
	SkelMeshComp2->SetupAttachment(RootComponent);

	SkelMeshComp3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp3"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef3(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/SHIP_EXTERIOR/SKM_SHIP_EXTERIOR_3.SKM_SHIP_EXTERIOR_3'"));
	if (SkelMeshRef3.Succeeded()) SkelMeshComp3->SetSkeletalMeshAsset(SkelMeshRef3.Object);
	SkelMeshComp3->SetupAttachment(RootComponent);

	SkelMeshComp4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp4"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef4(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/SHIP_EXTERIOR/SKM_SHIP_EXTERIOR_1.SKM_SHIP_EXTERIOR_4'"));
	if (SkelMeshRef4.Succeeded()) SkelMeshComp4->SetSkeletalMeshAsset(SkelMeshRef4.Object);
	SkelMeshComp4->SetupAttachment(RootComponent);

	//Turret_SkelMeshComp1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp5"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Turret_SkelMeshCompRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/TURRETS_OUTSIDE/GATLING_TURRET/SKM_Ship_Gatling_Turret.SKM_Ship_Gatling_Turret'"));
	//if (Turret_SkelMeshCompRef1.Succeeded()) Turret_SkelMeshComp1->SetSkeletalMeshAsset(Turret_SkelMeshCompRef1.Object);
	//Turret_SkelMeshComp1->SetupAttachment(RootComponent);
	//Turret_SkelMeshComp1->SetRelativeLocation(FVector(1042.855861, -2810.377304, 1914.374900));
	//Turret_SkelMeshComp1->SetRelativeRotation(FRotator(0.000000, 89.999999, 0.000000));

	//Turret_SkelMeshComp2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp6"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Turret_SkelMeshCompRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/TURRETS_OUTSIDE/GATLING_TURRET/SKM_Ship_Gatling_Turret.SKM_Ship_Gatling_Turret'"));
	//if (Turret_SkelMeshCompRef2.Succeeded()) Turret_SkelMeshComp2->SetSkeletalMeshAsset(Turret_SkelMeshCompRef2.Object);
	//Turret_SkelMeshComp2->SetupAttachment(RootComponent);
	//Turret_SkelMeshComp2->SetRelativeLocation(FVector(-1122.822352, -2810.377272, 1914.374900));
	//Turret_SkelMeshComp2->SetRelativeRotation(FRotator(0.000000, -90.0, 0.000000));

	Turret_MeshComp1 = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("SkelMeshComp5"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Turret_MeshCompRef1(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/TURRETS_OUTSIDE/GATLING_TURRET/SKM_Ship_Gatling_Turret.SKM_Ship_Gatling_Turret'"));
	if (Turret_MeshCompRef1.Succeeded()) Turret_MeshComp1->SetSkinnedAssetAndUpdate(Turret_MeshCompRef1.Object);
	Turret_MeshComp1->SetupAttachment(RootComponent);
	Turret_MeshComp1->SetRelativeLocation(FVector(1042.855861, -2810.377304, 1914.374900));
	Turret_MeshComp1->SetRelativeRotation(FRotator(0.000000, 89.999999, 0.000000));

	Turret_MeshComp2 = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("SkelMeshComp6"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Turret_MeshCompRef2(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/SUPER_DESTROYER/Props/SKELETAL_MESH/TURRETS_OUTSIDE/GATLING_TURRET/SKM_Ship_Gatling_Turret.SKM_Ship_Gatling_Turret'"));
	if (Turret_MeshCompRef2.Succeeded()) Turret_MeshComp2->SetSkinnedAssetAndUpdate(Turret_MeshCompRef2.Object);
	Turret_MeshComp2->SetupAttachment(RootComponent);
	Turret_MeshComp2->SetRelativeLocation(FVector(-1122.822352, -2810.377272, 1914.374900));
	Turret_MeshComp2->SetRelativeRotation(FRotator(0.000000, -90.0, 0.000000));
}

void AShip::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if (bRotateTurret)
	{
		float TurnSpeed = 150.0f; // 초당 회전 속도 (도)

		FRotator TurnBoneRot;
		FRotator YawBoneRot;
		if (bRotateYaw)
		{
			TurnBoneRot = Turret_MeshComp1->GetBoneRotationByName(TEXT("Turret_Rotate_TURN"), EBoneSpaces::ComponentSpace);

			FRotator RotateVal = FRotator::ZeroRotator;

			if (abs(TurnBoneRot.Yaw - TargetRot.Yaw) > .5f)
			{
				float Delta = FMath::FindDeltaAngleDegrees(TurnBoneRot.Yaw, TargetRot.Yaw);
				float MaxStep = TurnSpeed * DeltaTime;
				float ClampedDelta = FMath::Clamp(Delta, -MaxStep, MaxStep);
				RotateVal.Yaw = TurnBoneRot.Yaw + ClampedDelta;
			}
			else
			{
				RotateVal.Yaw = TargetRot.Yaw;

				bRotateYaw = false;
				bRotatePitch = true;
			}

			Turret_MeshComp1->SetBoneRotationByName(TEXT("Turret_Rotate_TURN"), RotateVal, EBoneSpaces::ComponentSpace);
		}

		if(bRotatePitch)
		{
			TurnBoneRot = Turret_MeshComp1->GetBoneRotationByName(TEXT("Turret_Rotate_TURN"), EBoneSpaces::WorldSpace);
			YawBoneRot = Turret_MeshComp1->GetBoneRotationByName(TEXT("Turret_Rotate_YAW"), EBoneSpaces::ComponentSpace);
			
			FRotator RotateVal = TurnBoneRot;

			if (abs(YawBoneRot.Roll - TargetRot.Pitch) > .5f)
			{
				float Delta = FMath::FindDeltaAngleDegrees(YawBoneRot.Roll, TargetRot.Pitch);
				float MaxStep = TurnSpeed * DeltaTime;
				float ClampedDelta = FMath::Clamp(Delta, -MaxStep, MaxStep);
				RotateVal.Roll = YawBoneRot.Roll + ClampedDelta;

				Turret_MeshComp1->SetBoneRotationByName(TEXT("Turret_Rotate_YAW"), RotateVal, EBoneSpaces::WorldSpace);
			}
			else
			{
				RotateVal.Roll = TargetRot.Pitch;

				Turret_MeshComp1->SetBoneRotationByName(TEXT("Turret_Rotate_YAW"), RotateVal, EBoneSpaces::WorldSpace);

				Turret_MeshComp1->RefreshBoneTransforms();

				OnResetSpawnLoc.Execute();

				bRotatePitch = false;
				bRotateTurret = false;
			}
		}
	}
}

void AShip::SetTurretTurn(FVector TargetLoc)
{
	FVector WorldToTarget = TargetLoc - Turret_MeshComp1->GetComponentLocation();
	WorldToTarget = WorldToTarget.GetSafeNormal();

	// 함선 기준 로컬 방향으로 변환
	FVector LocalDir = GetActorTransform().InverseTransformVectorNoScale(WorldToTarget);

	// 로컬 방향에서 회전 추출
	TargetRot = LocalDir.Rotation();

	bRotateTurret = true;
	bRotateYaw = true;
	bRotatePitch = false;
}