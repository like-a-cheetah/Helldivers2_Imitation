// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Gun.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

#include "Data/GunData.h"
#include "Bullet.h"
#include "Interface/PlayerControl.h"
#include "Animations/PlayerCharacterAnimInstance.h"

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	MuzzleFlashVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashVFX"));
	MuzzleFlashVFX->SetupAttachment(RootComponent);
	MuzzleFlashVFX->SetActive(false);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();

	if(GunData)
	{
		SetGunData();
		bCompleteReload = true;

		AudioComp = UGameplayStatics::SpawnSound2D(this, SW_Shot);
		if (AudioComp) AudioComp->Stop();

		Rounds = MaxRounds;

		FTransform Tr_Socket = SkelMeshComp->GetBoneTransform(MuzzleSocketName);
		MuzzleFlashVFX->SetWorldLocation(Tr_Socket.GetLocation());
		MuzzleFlashVFX->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireRemainTime -= DeltaTime;
	FireRemainTime = FMath::Clamp(FireRemainTime, 0.0f, FireRate);
}

void AGun::SetGunData()
{
	bBurst = GunData->bBurst;
	RecoilVal = GunData->RecoilVal;
	FireRate = GunData->FireRate / 3600.0f;
	MaxRounds = GunData->MaxRounds;
	BulletC = GunData->Bullet;
	MuzzleSocketName = GunData->MuzzleSocketName;
	MT_PlayerReload = GunData->MT_PlayerReload;
	GunReload = GunData->MT_GunReload;
	MinPitch = GunData->MinPitch;
	MaxPitch = GunData->MaxPitch;
	MinYaw = GunData->MinYaw;
	MaxYaw = GunData->MaxYaw;

	SW_Shot = GunData->SW_Shot;
}

void AGun::Recoil()
{
	IPlayerControl* Player = Cast<IPlayerControl>(GetOwner());
	float RecoilPitch = FMath::FRandRange(MinPitch, MaxPitch);
	float RecoilYaw = FMath::FRandRange(MinYaw, MaxYaw);
	if (Player)
		Player->Recoil(RecoilPitch, RecoilYaw);

	//ACharacter* Player = Cast<ACharacter>(GetOwner());
	//UAnimInstance* PlayerAnimInstance = Player->GetMesh()->GetAnimInstance();
	//UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerAnimInstance);
	//if (PlayerCharacterAnimInstance)
	//{
	//	PlayerCharacterAnimInstance->ProceduralRecoil(-5.f);
	//}
}

void AGun::Shot()
{
	if (FireRemainTime == 0.0f && Rounds > 0 && bCompleteReload)
	{
		FRotator Direction;
		FVector SpawnLocation;
		SkelMeshComp->GetSocketWorldLocationAndRotation(MuzzleSocketName, SpawnLocation, Direction);

		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = GetOwner();
		if (Cast<APawn>(GetOwner()))
		{
			SpawnParam.Instigator = Cast<APawn>(GetOwner());
		}
		
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletC, SpawnLocation, Direction, SpawnParam);
		IPlayerControl* Player = Cast<IPlayerControl>(GetOwner());
		if(Player) Bullet->OnHitEnemy.BindLambda([Player]() { Player->OnBulletEnemyHit.ExecuteIfBound(); });
		
		FireRemainTime = FireRate;
		//Rounds--;

		MuzzleFlashVFX->ActivateSystem(true);

		Recoil();

		//if(AudioComp) AudioComp->Play();

		//FTimerHandle TimeHandle;
	//	GetWorld()->GetTimerManager().SetTimer(TimeHandle, [this]() {
		//	if (Rounds == 0 && AudioComp) AudioComp->FadeOut(0.1f, 0.0f);
		//	}, 0.5f, false
		//);
	}
}

void AGun::Reload()
{
	Rounds = MaxRounds;
	//UE_LOG(LogTemp, Log, TEXT("Reload"));
}

bool AGun::IsBurst()
{
	return bBurst;
}

FVector AGun::GetImpactPoint()
{
	FName LaserSocketName = TEXT("laser_socket");
	FVector AimTraceStart;
	FRotator SocketWorldRotation;
	
	SkelMeshComp->GetSocketWorldLocationAndRotation(MuzzleSocketName, AimTraceStart, SocketWorldRotation);
	FVector AimTraceEnd = AimTraceStart + SocketWorldRotation.Vector() * 5000.0f;

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, AimTraceStart, AimTraceEnd, ECC_Pawn);

	const USkeletalMeshSocket* LaserSocket = SkelMeshComp->GetSocketByName(LaserSocketName);
	//if (LaserSocket)
	//{
	//	FVector LaserStart;
	//	FRotator LaserWorldRotation;
	//	SkelMeshComp->GetSocketWorldLocationAndRotation(LaserSocketName, LaserStart, LaserWorldRotation);
	//	FVector LaserEnd = LaserStart + LaserWorldRotation.Vector() * 5000.0f;
	//	DrawDebugLine(
	//		GetWorld(),
	//		LaserStart,
	//		LaserEnd,
	//		FColor::Green,
	//		false,
	//		0.01f,
	//		0,
	//		1.0f
	//	);
	//}

	if (bHit)
	{
		return HitResult.Location;
	}
	else
	{
		return AimTraceEnd;
	}
}

UAnimMontage* AGun::GetReloadMontage()
{
	return MT_PlayerReload;
}

void AGun::PlayReloadMontage()
{
	SkelMeshComp->PlayAnimation(GunReload, false);
	bCompleteReload = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			bCompleteReload = true;
		}, GunReload->GetPlayLength(), false);
}

bool AGun::IsCompleteReload()
{
	return bCompleteReload;
}

void AGun::StopShotSound()
{
	//if(AudioComp) AudioComp->FadeOut(0.1f, 0.0f);
}
