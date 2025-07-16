// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AcidComponent.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Monsters/Enemy.h"
#include "Objects/AcidBall.h"
#include "EnemyKey.h"

UAcidComponent::UAcidComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AAcidBall> AcidBallRef(TEXT("/Game/HellDivers2/Objects/BP_AcidBall.BP_AcidBall_C"));
	if (AcidBallRef.Class) AcidBall = AcidBallRef.Class;

	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.Owner = GetOwner();
	if (Cast<APawn>(GetOwner()))
	{
		SpawnParam.Instigator = Cast<APawn>(GetOwner());
	}
}

void UAcidComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAcidComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(CoolTimer);
	
	Super::EndPlay(EndPlayReason);
}

void UAcidComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (Current_AcidCoolTime > 0.0f)
	//{
	//	Current_AcidCoolTime -= DeltaTime;

	//	if (Current_AcidCoolTime <= 0.0f)
	//	{
	//		//델리게이트 발생
	//	}
	//}
}

void UAcidComponent::SpitAcid()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	FTransform MuzzleTransform = OwnerCharacter->GetMesh()->GetSocketTransform(TEXT("Acid_Muzzle"));	//이거 소켓 만드는거 안하면 오류나서 예외처리 필요

	AEnemy* Enemy = Cast<AEnemy>(SpawnParam.Owner);
	AActor* Target = Enemy->GetTarget();

	FVector DirectionVec = Target->GetActorLocation() - MuzzleTransform.GetLocation();
	FRotator DirectionRot = DirectionVec.Rotation();

	GetWorld()->SpawnActor<AAcidBall>(AcidBall, MuzzleTransform.GetLocation(), DirectionRot, SpawnParam);

	if (OnAcidCoolTimeEnd.IsBound())
	{
		OnAcidCoolTimeEnd.Execute(false);

		GetWorld()->GetTimerManager().SetTimer(
			CoolTimer,
			([this]()
				{
					OnAcidCoolTimeEnd.Execute(true);
				}),
			Max_AcidCoolTime,
			false
		);
	}
}

