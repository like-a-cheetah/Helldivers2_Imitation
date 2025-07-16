// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"

#include "Interface/GunInterface.h"

#include "Gun.generated.h"

/**
 * 
 */

UCLASS()
class HELLDIVERS2_API AGun : public AItem, public IGunInterface
{
	GENERATED_BODY()
	
public:
	AGun();

	void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	void SetGunData();

protected:
	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	TObjectPtr<class UGunData> GunData;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	uint8 bBurst : 1;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	float RecoilVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	float FireRate;
	float FireRemainTime;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	uint32 Rounds;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	uint32 MaxRounds;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	TSubclassOf<class ABullet> BulletC;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	TObjectPtr<UAnimMontage> MT_PlayerReload;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	TObjectPtr<UAnimationAsset> GunReload;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	float MaxPitch;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	float MinPitch;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	float MaxYaw;

	UPROPERTY(EditAnywhere, Category = WeaponInfo)
	float MinYaw;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UTexture> GunIcon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UTexture> RoundIcon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UTexture> RoundFillIcon;

	uint8 bCompleteReload : 1;

	UFUNCTION()
	void Recoil();

	UPROPERTY(EditAnywhere, Category = Sound)
	TObjectPtr<USoundWave> SW_Shot;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> MuzzleFlashVFX;

	void AttachToPlayer(bool bPickedUp) override;

protected:
	// IGunInterface
	float Shot() override;
	void Reload() override;
	bool IsBurst() override;
	FVector GetImpactPoint() override;
	FVector GetMuzzleLocation() override;
	UAnimMontage* GetReloadMontage() override;
	void PlayReloadMontage() override;
	bool IsCompleteReload() override;
	void StopShotSound() override;
	float GetRoundRatio() override;
	UTexture* GetGunIcon() override;
	UTexture* GetRoundIcon() override;
	UTexture* GetRoundFillIcon() override;
};
