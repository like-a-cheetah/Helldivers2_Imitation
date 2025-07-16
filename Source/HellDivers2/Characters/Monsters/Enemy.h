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

	//virtual void BeginActivity(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(BlueprintCallable)
	virtual void BeginActivity();

private:
	uint8 bOnceBeginPlayEvent : 1;

	FOnEnemyAttacked OnAttacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	FName AgentName;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()	
	virtual void OnOverlapAttackBone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//void StartBasicAttack(FOnMontageEnded OnMontageEnd);
	
	UPROPERTY()
	TObjectPtr<UClass> AnimInstClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> MT_Born;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Montage")
	TObjectPtr<UAnimMontage> MT_Die;

	UFUNCTION()
	virtual void Die();

	FVector BeginPatrolPos;

	uint8 bRefreshAnimRot : 1;

	float BodyRadius;
	float BodyHalfHeight;
	float AttachDamage;
	EEnemyMovementMode EnemyMovementMode;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSpriteComponent> PaperSpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UCapsuleComponent>> BoneCollisions;

	UPROPERTY()
	TObjectPtr<UCharacterStatComponent> Stat;

	float AttackDamage;

public:
	// IEnemyAIInterface을(를) 통해 상속됨
	FORCEINLINE float GetBodyRadius() override { return BodyRadius; }
	FORCEINLINE float GetBodyHalfHeight() override { return BodyHalfHeight; }
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
	FORCEINLINE void SetRefreshAnimRot(bool InbAnimRot) override { bRefreshAnimRot = InbAnimRot; }

	FORCEINLINE void SetBeginPatrolPoint(FVector InBeginPatrolPos) { BeginPatrolPos = InBeginPatrolPos; }

};
