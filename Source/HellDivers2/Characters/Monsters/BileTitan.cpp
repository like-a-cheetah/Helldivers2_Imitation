// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/BileTitan.h"

#include "PaperSpriteComponent.h"

#include "Characters/Components/AIC_BileTitan.h"
#include "Characters/Components/AcidComponent.h"

ABileTitan::ABileTitan()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIC_BileTitan::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(630.0, 577.116394f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/CHARACTERS/TERMINIDS/TITAN/SK_Titan_SIMPLE.SK_Titan_SIMPLE'"));
	if (SkelMeshRef.Succeeded()) GetMesh()->SetSkeletalMeshAsset(SkelMeshRef.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -480.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/HellDivers2/Characters/Enemys/BileTitan/ABP_BileTitan.ABP_BileTitan_C"));
	if (AnimInstanceClassRef.Succeeded())	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_DieRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/BileTitan/EditedAnimation/MT_Die.MT_Die'"));
	if (MT_DieRef.Object) MT_Die = MT_DieRef.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_SpitAcidRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/BileTitan/EditedAnimation/MT_SpitAcid.MT_SpitAcid'"));
	if (MT_SpitAcidRef.Object) MT_SpitAcid = MT_SpitAcidRef.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_BasicAttackRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/BileTitan/EditedAnimation/MT_BasicLeftAttack.MT_BasicLeftAttack'"));
	if (MT_BasicAttackRef.Object) MT_BasicAttack = MT_BasicAttackRef.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_RampageRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/BileTitan/EditedAnimation/MT_Rampage.MT_Rampage'"));
	if (MT_RampageRef.Object) MT_Rampage = MT_RampageRef.Object;

	AcidComp = CreateDefaultSubobject<UAcidComponent>(TEXT("AcidComp"));
	AcidComp->SetMax_AcidCoolTime(15.0f);

	BodyRadius = 700;
	AttachDamage = 10.0f;
	
	Stat->SetMaxHp(3000.0f);

	PaperSpriteComp->SetWorldScale3D(FVector(2.f, 1.0f, 2.f));
}

void ABileTitan::BeginPlay()
{
	Super::BeginPlay();
}

void ABileTitan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABileTitan::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AcidComp->DestroyComponent();

	Super::EndPlay(EndPlayReason);
}

void ABileTitan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABileTitan::StartSpitAcid(FOnMontageEnded OnMontageEnd)
{
	PlayAnimMontage(MT_SpitAcid);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);
}

void ABileTitan::StartBasicAttack(FOnMontageEnded OnMontageEnd)
{
	PlayAnimMontage(MT_BasicAttack);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);
}

void ABileTitan::StartRampage(FOnMontageEnded OnMontageEnd)
{
	PlayAnimMontage(MT_Rampage);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);
}

void ABileTitan::OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapAttackBone(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABileTitan::BeginActivity()
{
	Super::BeginActivity();

	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(BBKEY_ACID_READY, true);

			AcidComp->OnAcidCoolTimeEnd.BindLambda([BlackboardComp](bool bAcidReady) {
				BlackboardComp->SetValueAsBool(BBKEY_ACID_READY, bAcidReady);
				});
		}
	}

}

UAcidComponent* ABileTitan::GetAcidComp()
{
	return AcidComp;
}
