// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Grenade.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "Characters/Components/CharacterStatComponent.h"

AGrenade::AGrenade()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_B.P_Explosion_Big_B'"));
	if (ParticleRef.Object)
	{
		VFX = ParticleRef.Object;
	}

	FuseTime = 3.0f;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade::OnSplashBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCharacterStatComponent* CharacterStat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
	if (CharacterStat)
	{
		UE_LOG(LogTemp, Log, TEXT("Splash %s %s"), *OtherActor->GetName(), *OtherComp->GetName());

		CharacterStat->ApplyDamage(100.0f);

		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			FVector Force = OtherActor->GetActorLocation() - GetActorLocation();
			Force = Force.GetSafeNormal() * 2000.0f;

			Character->LaunchCharacter(Force, true, true);
		}
	}
}

void AGrenade::Bomb()
{	

	FTransform SplashTransform;
	SplashTransform.SetLocation(GetActorLocation());
	SplashTransform.SetScale3D(FVector(1.8f, 1.8f, 1.8f));

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX, SplashTransform);

	//USphereComponent* Splash = NewObject<USphereComponent>(this, TEXT("Splash"));
	USphereComponent* Splash = NewObject<USphereComponent>(UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX, SplashTransform));

	Splash->SetCollisionProfileName(TEXT("Splash"));
	Splash->SetWorldLocation(GetActorLocation());
	Splash->SetSphereRadius(500.0f);
	Splash->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnSplashBeginOverlap);

	Splash->SetVisibility(true);
	Splash->SetHiddenInGame(false);

	Splash->RegisterComponent();
	Splash->UpdateOverlaps();

	Destroy();
}

void AGrenade::PullingPin()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGrenade::Bomb, FuseTime, false);
}
