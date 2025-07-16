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
		// 데이터 애셋 값 복사
		SetGunData();
		bCompleteReload = true;

		// 풀 탄창으로 시작
		Rounds = MaxRounds;

		// 총구 소켓 위치에서 발사 지점 위치 조정
		FTransform Tr_Socket = SkelMeshComp->GetBoneTransform(MuzzleSocketName);
		MuzzleFlashVFX->SetWorldLocation(Tr_Socket.GetLocation());
		MuzzleFlashVFX->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireRemainTime -= DeltaTime;
	FireRemainTime = FMath::Clamp(FireRemainTime, 0.0f, FireRate);
}

// 데이터 애셋 프로퍼티 값 복사
void AGun::SetGunData()
{
	// 연사 기능 유무
	bBurst = GunData->bBurst;					
	// GunData->FireRate는 무기의 분당 발사수(RPM)이므로, 한 발 발사 후 대기 시간(초)로 변환
	FireRate = 60.0f / GunData->FireRate;		
	// 탄창 수
	MaxRounds = GunData->MaxRounds;

	// 발사에 사용할 총알의 서브클래스
	BulletC = GunData->Bullet;
	// 총구 소켓 이름
	MuzzleSocketName = GunData->MuzzleSocketName;

	// 플레이어 리로드 몽타주 애셋
	MT_PlayerReload = GunData->MT_PlayerReload;
	// 무기 리로드 몽타주 애셋
	GunReload = GunData->MT_GunReload;

	// 총기 반동 최소 Pitch 값
	MinPitch = GunData->MinPitch;
	// 총기 반동 최대 Pitch 값 
	MaxPitch = GunData->MaxPitch;

	// 총기 반동 최소 Yaw 값 복사
	MinYaw = GunData->MinYaw;
	// 총기 반동 최대 Yaw 값 복사
	MaxYaw = GunData->MaxYaw;

	// 무기 실루엣 아이콘 텍스처
	GunIcon = GunData->GunIcon;
	// 탄창 아이콘 텍스처 
	RoundIcon = GunData->RoundIcon;
	// 탄창 게이지 아이콘 텍스처
	RoundFillIcon = GunData->RoundFillIcon;

	// 발사 사운드 애셋
	SW_Shot = GunData->SW_Shot;
}

// 무기 반동 적용 (Pitch, Yaw 범위 내에서 랜덤값 생성하여 PlayerControl 인터페이스로 전달)
void AGun::Recoil()
{
	IPlayerControl* Player = Cast<IPlayerControl>(GetOwner());
	float RecoilPitch = FMath::FRandRange(MinPitch, MaxPitch);
	float RecoilYaw = FMath::FRandRange(MinYaw, MaxYaw);
	if (Player)
		Player->Recoil(RecoilPitch, RecoilYaw);
}

// 무기 오브젝트를 플레이어에 부착
void AGun::AttachToPlayer(bool bPickedUp)
{
	Super::AttachToPlayer(bPickedUp);

	if (bPickedUp)
	{
		if (BulletC)
		{
			IPlayerControl* Player = Cast<IPlayerControl>(GetOwner());
			// 총알 적중시, 플레이어 UI 표시 이벤트 바인딩
			if (Player) ABullet::OnHitEnemy.BindLambda([Player]() { Player->OnBulletEnemyHit.ExecuteIfBound(); });
		}
	}
}

// 무기 발사 처리
float AGun::Shot()
{
	// 발사 쿨타임 종료, 탄알 존재, 재장전이 완료됐을 때
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
		
		FireRemainTime = FireRate;
		Rounds--;

		MuzzleFlashVFX->ActivateSystem(true);

		Recoil();
	}

	return (float)Rounds / MaxRounds;
}

// 탄창 재장전, 탄 수를 최대치로 복원
void AGun::Reload()
{
	Rounds = MaxRounds;
}

// 마우스 트리거시 연사 기능 사용 여부 반환
bool AGun::IsBurst()
{
	return bBurst;
}

// 총구를 기준으로, 라인 트레이스를 사용해 실제 피격 위치 계산
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

	// 트래이스 충돌시
	if (bHit)
	{
		// 충돌 지점 반환
		return HitResult.Location;
	}
	else // 트래이스 미충돌시
	{
		// 트레이스 끝 지점 반환
		return AimTraceEnd;
	}
}

// 플레이어 리로드 몽타주 반환
UAnimMontage* AGun::GetReloadMontage()
{
	return MT_PlayerReload;
}

// 무기 리로드 애니메이션 재생
void AGun::PlayReloadMontage()
{
	SkelMeshComp->PlayAnimation(GunReload, false);
}

// 재장전 완료 여부 반환
bool AGun::IsCompleteReload()
{
	return bCompleteReload;
}

// 총알 발사 사운드 정지 (개선 필요)
void AGun::StopShotSound()
{
	//if(AudioComp) AudioComp->FadeOut(0.1f, 0.0f);
}

// UI표시를 위한 현재 탄창 비율 반환
float AGun::GetRoundRatio()
{
	return (float)Rounds / MaxRounds;
}

UTexture* AGun::GetGunIcon()
{
	return GunIcon;
}

UTexture* AGun::GetRoundIcon()
{
	return RoundIcon;
}

UTexture* AGun::GetRoundFillIcon()
{
	return RoundFillIcon;
}

// (미 사용) 총구(Muzzle) 위치 반환 (소켓이 있는 경우)
FVector AGun::GetMuzzleLocation()
{
	FVector MuzzleLocation;
	if(SkelMeshComp) MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
	return MuzzleLocation;
}
