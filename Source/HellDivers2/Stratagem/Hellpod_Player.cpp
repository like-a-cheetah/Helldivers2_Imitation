// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem/Hellpod_Player.h"


#include "Characters/Player/PlayerCharacter.h"

AHellpod_Player::AHellpod_Player()
{
}

void AHellpod_Player::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(GetWorld()->GetFirstPlayerController());
	InputComponent->BindAxis("MoveX", this, &AHellpod_Player::MoveX);
	InputComponent->BindAxis("MoveY", this, &AHellpod_Player::MoveY);
}

void AHellpod_Player::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnMeshOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);
}

void AHellpod_Player::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnBoxOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);
}

void AHellpod_Player::SpawnAttachMachine()
{
	Super::SpawnAttachMachine();

	HellpodMesh->PlayAnimation(MT_RecallPlayer, false);

	if (OnPlayerArrive.IsBound()) OnPlayerArrive.Execute();
}

void AHellpod_Player::AttchPlayer(APlayerCharacter* Player)
{
	FAttachmentTransformRules CustomRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Player->AttachToActor(this, CustomRules, FName(TEXT("footrest_Socket")));

	OnPlayerArrive.BindUObject(Player, &APlayerCharacter::PlayerRebirth);
}

void AHellpod_Player::MoveX(float Value)
{
	if (!CheckOnce)
	{
		float ForceMagnitude = Value * 200000.0f;
		HellpodMesh->AddForce(FVector(0.0f, ForceMagnitude, 0.0f));
	}
}

void AHellpod_Player::MoveY(float Value)
{
	if (!CheckOnce)
	{
		float ForceMagnitude = Value * 200000.0f;
		HellpodMesh->AddForce(FVector(ForceMagnitude, 0.0f, 0.0f));
	}
}