// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/Charger.h"

#include "PaperSpriteComponent.h"

#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Components/AIC_Charger.h"

ACharger::ACharger()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIC_Charger::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(213.589035f, 213.589035f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	AttachDamage = 10.0f;
	BodyRadius = 270.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Characters/TERMINIDS/CHARGER/SK_CHARGER_02_SIMPLE.SK_CHARGER_02_SIMPLE'"));
	if (SkelMeshRef.Succeeded()) GetMesh()->SetSkeletalMeshAsset(SkelMeshRef.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(30.0f, 0.0f, -180.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/HellDivers2/Characters/Enemys/Charger/ABP_Charger.ABP_Charger_C"));
	if (AnimInstanceClassRef.Succeeded())	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	BodySlamCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodySlamCollider"));
	BodySlamCollider->SetupAttachment(GetMesh(), TEXT("head1"));
	BodySlamCollider->OnComponentBeginOverlap.AddDynamic(this, &ACharger::OnOverlapBodySlam);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_DieRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/Charger/EditedAnimation/MT_Die.MT_Die'"));
	if (MT_DieRef.Object) MT_Die = MT_DieRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_StartRushRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/Charger/EditedAnimation/MT_StartRush.MT_StartRush'"));
	if (MT_StartRushRef.Object) MT_StartRush = MT_StartRushRef.Object;

	PaperSpriteComp->SetWorldScale3D(FVector(1.5f, 1.0f, 1.5f));
}

void ACharger::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetMaxHp(1200.0f);

}

void ACharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharger::OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapAttackBone(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		FVector Direct = OtherActor->GetActorLocation() - GetActorLocation();
		Direct.Normalize();
		Direct.Z = 1.f;
		Direct *= 200.f;

		Player->LaunchCharacter(Direct, false, false);
	}
}

void ACharger::OnOverlapBodySlam(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapAttackBone(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		FVector Dir = Player->GetActorLocation() - GetActorLocation();
		Dir.Normalize();
		Dir *= 500.f;
		Dir.Z = 800.0f;

		Player->LaunchCharacter(Dir, false, false);
		Player->Stun(5.0f);
	}
}

void ACharger::Rush()
{
	EnemyMovementMode = EEnemyMovementMode::Rush;
}

void ACharger::ArmorBreak()
{
}
