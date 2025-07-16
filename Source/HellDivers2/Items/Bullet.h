// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Bullet.generated.h"

DECLARE_DELEGATE(FOnHitEnemy);

UCLASS()
class HELLDIVERS2_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	static FOnHitEnemy OnHitEnemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Trail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HitVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	float HitVFXScale;

	//UFUNCTION()
	//virtual void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void HitPostProcess(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	float Damage;
};
