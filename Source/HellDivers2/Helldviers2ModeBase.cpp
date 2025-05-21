// Fill out your copyright notice in the Description page of Project Settings.


#include "Helldviers2ModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

#include "Characters/Player/DiversPlayerController.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Stratagem/Hellpod_Player.h"

AHelldviers2ModeBase::AHelldviers2ModeBase()
{
	PlayerControllerClass = ADiversPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<AHellpod> HellPodPlayer_C_Ref(TEXT("/Game/HellDivers2/Stratagem/HellPodPlayer/BP_Hellpod.BP_Hellpod_C"));
	if (HellPodPlayer_C_Ref.Class) HellPodPlayer_C = HellPodPlayer_C_Ref.Class;
}

AActor* AHelldviers2ModeBase::ChoosePlayerStart_Implementation(AController* Controller)
{
    APlayerStart* PlayerStart = Cast<APlayerStart>(FindPlayerStart(Controller, TEXT("test")));
	PlayerStart->GetCapsuleComponent()->SetMobility(EComponentMobility::Movable);

	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName == "InGameTestmap")
	{
		ResetSpawnPoint(PlayerStart);
	}
	else
	{
	}

    return PlayerStart;
}

void AHelldviers2ModeBase::BeginPlay()
{
	Super::BeginPlay();

	ADiversPlayerController* PlayerController = Cast<ADiversPlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (PlayerChar)
	{
		FString LevelName = GetLevel()->GetOuter()->GetName();
		if (LevelName == "InGameTestmap")
		{
			PlayerChar->OnRespawnPlayer.BindUObject(this, &AHelldviers2ModeBase::SpawnPlayerHellpod);

			////실제 게임 시뮬
			//SpawnPlayerHellpod(PlayerChar);
			////실제 게임 시뮬
		}
	}
}

void AHelldviers2ModeBase::ResetSpawnPoint(AActor* Actor)
{
	StartPos = Actor->GetActorLocation();
	StartPos.Z += 10000.0f;
	//PlayerStart->SetActorLocation(StartPos);
	StartRot = Actor->GetActorRotation();
	StartRot.Yaw += 60.0f;
}

void AHelldviers2ModeBase::SpawnPlayerHellpod(APlayerCharacter* PlayerChar)
{
	AHellpod_Player* HellPod = GetWorld()->SpawnActor<AHellpod_Player>(HellPodPlayer_C, StartPos, StartRot);
	HellPod->AttchPlayer(PlayerChar);
}
