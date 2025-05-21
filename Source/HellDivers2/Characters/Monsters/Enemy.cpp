// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/Enemy.h"

#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"

#include "Characters/Components/AIController_Enemy.h"
#include "Animations/AnimInst_Enemy.h"

FOnEnemyBeginPlay AEnemy::OnEnemyBeginPlay;
//FOnEnemyDie AEnemy::OnEnemyDie;

AEnemy::AEnemy()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->BodyInstance.bLockRotation = true;
	GetCapsuleComponent()->SetCanEverAffectNavigation(true);

	GetMesh()->SetCollisionProfileName("Enemy");
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetGenerateOverlapEvents(false);

	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(100.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	BodyRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	PatrolRadius = 5000.0f;

	EnemyMovementMode = EEnemyMovementMode::Idle;

	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));
	Stat->OnHpZero.AddDynamic(this, &AEnemy::Die);

	AttackDamage = 5.0f;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!bOnceBeginPlayEvent)
	{
		OnEnemyBeginPlay.Broadcast(this);
		bOnceBeginPlayEvent = true;
	}

	TArray<UCapsuleComponent*> CapsuleComponents;
	GetComponents(CapsuleComponents);

	for (UCapsuleComponent* CapsuleComp : CapsuleComponents)
	{
		if (CapsuleComp != RootComponent)
		{
			CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapAttackBone);

			BoneCollisions.Add(*CapsuleComp->GetName(), CapsuleComp);
		}
	}

	if (MT_Born)
	{
		PlayAnimMontage(MT_Born);

		FOnMontageEnded OnBornMTEnd;
		OnBornMTEnd.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) { BeginActivity(); });
		//OnBornMTEnd.BindUObject(this, &AEnemy::BeginActivity);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnBornMTEnd, MT_Born);
	}
	else
	{
		BeginActivity();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::BeginActivity()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCharacterMovement()->GravityScale = 1.0f;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetNotifyRigidBodyCollision(true);
	//GetMesh()->SetSimulatePhysics(true);

	AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass);
	AIController->Possess(this);
	AIController->GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, BeginPatrolPos);


	AAIController_Enemy* EnemyAIController = Cast<AAIController_Enemy>(GetController());

	OnEnemyDie.AddLambda([EnemyAIController]() { EnemyAIController->StopAI(); });

	OnAttacked.BindLambda([EnemyAIController](AActor* Attacker) { EnemyAIController->SetTarget(Attacker); });
}

void AEnemy::OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//UE_LOG(LogTemp, Log, TEXT("%s's %s hit Player"), *GetName(), *OverlappedComponent->GetName());

		UCharacterStatComponent* PlayerStat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
		if (PlayerStat) PlayerStat->ApplyDamage(AttackDamage);
	}
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	OnAttacked.Execute(DamageCauser);

	return Damage;
}

void AEnemy::Die()
{
	PlayAnimMontage(MT_Die);

	GetMesh()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	Stat->DestroyComponent();

	OnEnemyDie.Broadcast();
}

void AEnemy::SetBoneColloisionActivity(FName BoneName, bool bActive)
{
	if (!BoneCollisions.Find(BoneName))
		return;

	if (bActive)
	{
		BoneCollisions[BoneName]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BoneCollisions[BoneName]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemy::SetWalk()
{
	EnemyMovementMode = EEnemyMovementMode::Walk;
}

void AEnemy::SetTrace()
{
	EnemyMovementMode = EEnemyMovementMode::Trace;
}

void AEnemy::SetStop()
{
	EnemyMovementMode = EEnemyMovementMode::Idle;
}

void AEnemy::SetAttack()
{
	EnemyMovementMode = EEnemyMovementMode::Attack;
}

//void AEnemy::StartBasicAttack(FOnMontageEnded OnMontageEnd)
//{
//	if (MT_BasicAttack)
//	{
//		PlayAnimMontage(MT_BasicAttack);
//		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);
//	}
//}

void AEnemy::MontagePlay_SetEndDelegate(UAnimMontage* Montage, FOnMontageEnded OnMontageEnd)
{
	SetStop();

	PlayAnimMontage(Montage);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);
}


void AEnemy::SetRotate(float Angle)
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	Cast<UAnimInst_Enemy>(AnimInst)->SetRotateAngle(Angle);
}
