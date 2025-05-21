// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/Warrior.h"

#include "Characters/Components/AIC_Warrior.h"

AWarrior::AWarrior()
{
	AIControllerClass = AAIC_Warrior::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(152, 152);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Characters/TERMINIDS/WARRIOR/SK_WARRIOR_03_SIMPLE.SK_WARRIOR_03_SIMPLE'"));
	if (SkelMeshRef.Succeeded()) GetMesh()->SetSkeletalMeshAsset(SkelMeshRef.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -137.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/HellDivers2/Characters/Enemys/Warrior/ABP_Warrior.ABP_Warrior_C"));
	if (AnimInstanceClassRef.Succeeded())	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_DieRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/Warrior/EditedAnimation/MT_Die.MT_Die'"));
	if (MT_DieRef.Object) MT_Die = MT_DieRef.Object;

	AttachDamage = 10.0f;
	BodyRadius = 270.0f;
}
