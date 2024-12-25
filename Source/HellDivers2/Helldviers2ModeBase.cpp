// Fill out your copyright notice in the Description page of Project Settings.


#include "Helldviers2ModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Player/DiversPlayerController.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Stratagem/HellPodPlayer.h"

AHelldviers2ModeBase::AHelldviers2ModeBase()
{
	PlayerControllerClass = ADiversPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<AHellPodPlayer> HellPodPlayer_C_Ref(TEXT("/Game/HellDivers2/Stratagem/HellPodPlayer/MyHellPodPlayer.MyHellPodPlayer_C"));
	if (HellPodPlayer_C_Ref.Class) HellPodPlayer_C = HellPodPlayer_C_Ref.Class;
}

AActor* AHelldviers2ModeBase::ChoosePlayerStart_Implementation(AController* Controller)
{
	StartRot = FRotator(0.0f, 0.0f, 0.0f);
	StartPos = FVector(0.0f, 0.0f, 0.0f);

    APlayerStart* PlayerStart = Cast<APlayerStart>(FindPlayerStart(Controller, TEXT("test")));
	if (PlayerStart)
	{
		StartPos = PlayerStart->GetActorLocation();
		StartRot = PlayerStart->GetActorRotation();
	}

	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName == "InGameTestmap") StartPos.Z += 20000.0f;

    return Super::ChoosePlayerStart_Implementation(Controller);
}

void AHelldviers2ModeBase::BeginPlay()
{
	Super::BeginPlay();

	ADiversPlayerController* PlayerController = Cast<ADiversPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	APlayerCharacter* PlayerChar = GetWorld()->SpawnActor<APlayerCharacter>(StartPos, StartRot);
	PlayerController->Possess(PlayerChar);

	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName == "InGameTestmap")
	{
		AHellPodPlayer* HellPod = GetWorld()->SpawnActor<AHellPodPlayer>(HellPodPlayer_C, StartPos, StartRot);
		HellPod->AttchPlayer(PlayerChar);
	}
}