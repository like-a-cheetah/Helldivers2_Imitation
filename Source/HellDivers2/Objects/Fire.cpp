// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Fire.h"

#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Characters/Components/CharacterStatComponent.h"
#include "FireSubsystem.h"

//TMap<UCharacterStatComponent*, TPair<int32, float>> AFire::OverlappedCharacters;

AFire::AFire()
{
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetCapsuleSize(150.f, 150.f);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnFireOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AFire::OnFireOverlapEnd);
	Collider->SetCollisionProfileName(TEXT("BlockStaticAndOverlapChar"));
	RootComponent = Collider;

	PS_Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire"));
	PS_Fire->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticleRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big1.P_Fire_Big1'"));
	if (FireParticleRef.Succeeded()) PS_Fire->SetTemplate(FireParticleRef.Object);
}

//void AFire::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	for (auto Character = OverlappedCharacters.CreateIterator(); Character; ++Character)
//	{
//		if (Character.Value().Key <= 0)
//			Character.RemoveCurrent();
//		else
//		{
//			if (Character.Key() && Character.Value().Value <= 0.0f)
//			{
//				Character.Key()->ApplyDamage(5.0f * DeltaTime);
//
//			    Character.Value().Value = DamageCooldownTime;
//			}
//			else
//				Character.Value().Value -= DeltaTime;
//		}
//	}
//}

void AFire::BeginPlay()
{
	Super::BeginPlay();

	FireSubsystem = GetWorld()->GetSubsystem<UFireSubsystem>();

	FTimerHandle OnFireOff;
	GetWorld()->GetTimerManager().SetTimer(OnFireOff, this, &AFire::OffFire, 4.0f, false);
}

void AFire::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AFire::OnFireOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCharacterStatComponent* Stat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
	if (Stat) FireSubsystem->RegisterOverlappedCharacter(Stat);		
}

void AFire::OnFireOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UCharacterStatComponent* Stat = OtherActor->FindComponentByClass<UCharacterStatComponent>();
	if (Stat) FireSubsystem->OverlapEndCharacter(Stat);
}

void AFire::OffFire()
{
	PS_Fire->Deactivate();

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TSet<AActor*> CurrentOverlappingActor;
	Collider->GetOverlappingActors(CurrentOverlappingActor);

	for (AActor* Actor : CurrentOverlappingActor)
	{
		UCharacterStatComponent* Stat = Actor->FindComponentByClass<UCharacterStatComponent>();

		if (Stat) FireSubsystem->OverlapEndCharacter(Stat);
	}
}
