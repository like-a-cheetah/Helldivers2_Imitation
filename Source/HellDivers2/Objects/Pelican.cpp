// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Pelican.h"

#include "Kismet/GameplayStatics.h"

#include "Interface/PlayerControl.h"

APelican::APelican()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/PROJECTS/HELLDIVERS_2/Props/GAMEPLAY/PELICAN-1/SK_PELICAN-1.SK_PELICAN-1'"));
	SkelMeshComp->SetSkeletalMesh(MeshAsset.Object);
	SkelMeshComp->SetCollisionProfileName(TEXT("BlockAll"));

	static ConstructorHelpers::FObjectFinder<UAnimSequence> AS_EndGameRef(TEXT("/Script/Engine.AnimSequence'/Game/PROJECTS/HELLDIVERS_2/Props/GAMEPLAY/PELICAN-1/Animations/MissionEnd_A_0x3637b00204b5b2ac_FLY1.MissionEnd_A_0x3637b00204b5b2ac_FLY1'"));
	if (AS_EndGameRef.Object) AS_EndGame = AS_EndGameRef.Object;
}

void APelican::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DoorLoc = SkelMeshComp->GetBoneLocation(TEXT("passenger_conduit"));
	DoorLoc.Z += 100.0f;
	TiggerCollision->SetWorldLocation(DoorLoc);
}

void APelican::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	FTransform AttachTr = SkelMeshComp->GetSocketTransform(TEXT("l_f_seatbelt_Socket"));

	IPlayerControl* Player = Cast<IPlayerControl>(OtherActor);
	if (Player)
	{ 
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, false);
		OtherActor->AttachToActor(this, Rule, TEXT("l_f_seatbelt_Socket"));

		Player->RidePelican(AttachTr);

		FVector Offset = GetActorLocation();
		Offset.Z -= 270.0f;
		SetActorLocation(Offset);

		SkelMeshComp->PlayAnimation(AS_EndGame, false);

		FTimerHandle OnPelicanFlyEnd;
		GetWorld()->GetTimerManager().SetTimer(OnPelicanFlyEnd, [this]() {
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestShip"));
			}, 8.0f, false);
		
		//float RideEndTime = Player->RidePelican(AttachTr);

		//FTimerHandle PlayerRideEnd;
		//GetWorld()->GetTimerManager().SetTimer(
		//	PlayerRideEnd, 
		//	[this]() 
		//	{ 
		//		FVector Offset = GetActorLocation();
		//		Offset.Z -= 270.0f;
		//		SetActorLocation(Offset);

		//		SkelMeshComp->PlayAnimation(AS_EndGame, false);

		//		FTimerHandle OnPelicanFlyEnd;
		//		GetWorld()->GetTimerManager().SetTimer(OnPelicanFlyEnd, [this]() {
		//			UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestShip"));
		//			}, 8.0f, false);

		//		bRotate = true;
		//	}, 
		//	RideEndTime, false );
	}
}


