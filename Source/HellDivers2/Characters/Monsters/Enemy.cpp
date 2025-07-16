// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/Enemy.h"

#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "NavigationSystem.h"

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
	GetMesh()->bHiddenInSceneCapture = true;

	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(100.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	PaperSpriteComp->SetupAttachment(RootComponent);
	PaperSpriteComp->bVisibleInSceneCaptureOnly = true;
	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(TEXT("/Script/Paper2D.PaperSprite'/Game/HellDivers2/UI/InGame/PlayerSprite_Sprite.PlayerSprite_Sprite'"));
	if (SpriteAsset.Succeeded()) PaperSpriteComp->SetSprite(SpriteAsset.Object);
	PaperSpriteComp->SetSpriteColor(FColor::Red);
	PaperSpriteComp->SetRelativeLocation(FVector(-0.000000, 0.000057, 4669.000000));
	PaperSpriteComp->SetRelativeRotation(FRotator(0, 0, -90.0f));
	PaperSpriteComp->SetCollisionProfileName(TEXT("NoCollision"));
	PaperSpriteComp->SetCanEverAffectNavigation(false);
	PaperSpriteComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		FVector NewScale = MeshComp->Bounds.BoxExtent;
		PaperSpriteComp->SetWorldScale3D(FVector(NewScale.X, 1.f, NewScale.Y));
	}
	PatrolRadius = 5000.0f;

	EnemyMovementMode = EEnemyMovementMode::Idle;

	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));
	Stat->OnHpZero.AddDynamic(this, &AEnemy::Die);

	AttackDamage = 5.0f;

	bRefreshAnimRot = true;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	ANavigationData* AgentData = NavSys->GetNavDataForAgentName(AgentName);
	GetCharacterMovement()->NavAgentProps = AgentData->GetConfig();

	BodyRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	BodyHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

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
			CapsuleComp->SetCollisionProfileName(TEXT("EnemyOverlapAttack"));
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if(!CapsuleComp->OnComponentBeginOverlap.IsBound()) CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapAttackBone);

			BoneCollisions.Add(*CapsuleComp->GetName(), CapsuleComp);
		}
	}

	if (MT_Born)
	{
		PlayAnimMontage(MT_Born);

		FOnMontageEnded OnBornMTEnd;
		OnBornMTEnd.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) { BeginActivity(); });
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

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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

	if (BeginPatrolPos.IsZero())  BeginPatrolPos = GetActorLocation();
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
	
	OnAttacked.ExecuteIfBound(DamageCauser);

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
	//if (bRefreshAnimRot)
	{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
		Cast<UAnimInst_Enemy>(AnimInst)->SetRotateAngle(Angle);
	}
}
