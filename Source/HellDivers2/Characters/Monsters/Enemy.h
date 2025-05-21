// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/EnemyAIInterface.h"
#include "Interface/AnimMovementInterface.h"
#include "Characters/Components/CharacterStatComponent.h"

#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementMode : uint8
{
	Idle = 0,
	Walk,
	Trace,
	Die,
	Rush,
	Attack
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyBeginPlay, APawn*);
DECLARE_MULTICAST_DELEGATE(FOnEnemyDie);
DECLARE_DELEGATE_OneParam(FOnEnemyAttacked, AActor* /*Player*/);

UCLASS()
class HELLDIVERS2_API AEnemy : public ACharacter, public IEnemyAIInterface, public IAnimMovementInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	static FOnEnemyBeginPlay OnEnemyBeginPlay;
	//static FOnEnemyDie OnEnemyDie;
	FOnEnemyDie OnEnemyDie;

	FORCEINLINE EEnemyMovementMode GetEnemyMovementMode() { return EnemyMovementMode; }

private:
	uint8 bOnceBeginPlayEvent : 1;

	FOnEnemyAttacked OnAttacked;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()	
	virtual void OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//void StartBasicAttack(FOnMontageEnded OnMontageEnd);
	
	TObjectPtr<UClass> AnimInstClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> MT_Born;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Montage")
	TObjectPtr<UAnimMontage> MT_Die;

	//virtual void BeginActivity(UAnimMontage* Montage, bool bInterrupted);
	virtual void BeginActivity();

	UFUNCTION()
	virtual void Die();

	FVector BeginPatrolPos;

	float BodyRadius;
	float AttachDamage;
	EEnemyMovementMode EnemyMovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UCapsuleComponent>> BoneCollisions;

	TObjectPtr<UCharacterStatComponent> Stat;

	float AttackDamage;

public:
	// IEnemyAIInterface을(를) 통해 상속됨
	FORCEINLINE float GetBodyRadius() override { return BodyRadius; }
	FORCEINLINE float GetPatrolRadius() override { return PatrolRadius; }
	FORCEINLINE void SetTarget(AActor* InTarget) override { Target = InTarget; }
	FORCEINLINE AActor* GetTarget() override { return Target; }

public:
	virtual void SetBoneColloisionActivity(FName BoneName, bool bActive);

	// IAnimMovementInterface을(를) 통해 상속됨
	void SetWalk() override;
	void SetTrace() override;
	void SetStop() override;
	void SetAttack() override;
	void MontagePlay_SetEndDelegate(UAnimMontage* Montage, FOnMontageEnded OnMontageEnd);
	void SetRotate(float Angle) override;

	FORCEINLINE void SetBeginPatrolPoint(FVector InBeginPatrolPos) { BeginPatrolPos = InBeginPatrolPos; }

};
