// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monsters/Hunter.h"

#include "Characters/Components/AIC_Hunter.h"

AHunter::AHunter()
{
	AIControllerClass = AAIC_Hunter::StaticClass();

	GetCapsuleComponent()->InitCapsuleSize(176.456177f, 165.824036);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Characters/TERMINIDS/HUNTER/SK_HUNTER_SIMPLE.SK_HUNTER_SIMPLE'"));
	if (SkelMeshRef.Succeeded()) GetMesh()->SetSkeletalMeshAsset(SkelMeshRef.Object);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -137.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/HellDivers2/Characters/Enemys/Hunter/ABP_Hunter.ABP_Hunter_C"));
	if (AnimInstanceClassRef.Succeeded())	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_DieRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Enemys/Hunter/EditedAnimation/MT_Die.MT_Die'"));
	if (MT_DieRef.Object) MT_Die = MT_DieRef.Object;

	AcidComp = CreateDefaultSubobject<UAcidComponent>(TEXT("AcidComp"));
	AcidComp->SetMax_AcidCoolTime(8.0f);

	AttachDamage = 10.0f;
	BodyRadius = 270.0f;
	
	AttackDamage = 30.0f;
}

UAcidComponent* AHunter::GetAcidComp()
{
	return AcidComp;
}

void AHunter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AcidComp->DestroyComponent();

	Super::EndPlay(EndPlayReason);
}

void AHunter::BeginActivity()
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
