#include "Objects/BugHole.h"

#include "Components/SphereComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemComponent.h"                           // UFieldSystemComponent (선택사항)
#include "Field/FieldSystemObjects.h"                             // URadialFalloff, UUniformVector, UOperatorField 등
#include "Field/FieldSystemTypes.h"    

#include "Characters/Monsters/Enemy.h"
#include "Characters/Components/CharacterStatComponent.h"

FOnBugHoleBeginPlay ABugHole::OnBugHoleBeginPlay;

ABugHole::ABugHole()
{
	PrimaryActorTick.bCanEverTick = true;

	//SM_Hole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Hole"));
	//SM_Hole->SetCollisionProfileName(TEXT("BlockAll"));
	//SM_Hole->SetGenerateOverlapEvents(true);
	
	GCC_Mesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GCC_Mesh"));
	GCC_Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	GCC_Mesh->SetGenerateOverlapEvents(true);
	GCC_Mesh->ObjectType = EObjectStateTypeEnum::Chaos_Object_Dynamic;
	GCC_Mesh->bEnableDamageFromCollision = false;

	RootComponent = GCC_Mesh;

	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));
	Stat->OnHpZero.AddDynamic(this, &ABugHole::HoleBroken);

	SpawnPatrolPos = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnPatrolPos"));
	SpawnPatrolPos->SetupAttachment(RootComponent);

	MaxSpawnTime = 15.0f;
}

void ABugHole::BeginPlay()
{
	Super::BeginPlay();

	if (!bOnceBeginPlayEvent)
	{
		OnBugHoleBeginPlay.ExecuteIfBound(this);
		bOnceBeginPlayEvent = true;
	}
}

void ABugHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bBroken) return;

	SpawnTime -= DeltaTime;
	if (SpawnTime <= 0.0f)
	{
		SpawnMonster();
		SpawnTime = MaxSpawnTime;
	}
}

void ABugHole::SpawnMonster()
{
	if (!bBroken && SpawnSphere && MonsterN < 8)
	{
		FTransform SpawnOffset = SpawnSphere->GetComponentTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(Monster, SpawnOffset, SpawnParams);
		if (Enemy)
		{
			MonsterN++;

			Enemy->SetBeginPatrolPoint(SpawnPatrolPos->GetComponentLocation());

			Enemy->OnEnemyDie.AddLambda([this]() { MonsterN--; });
		}
	}
}

void ABugHole::HoleBroken()
{
	bBroken = true;

	GCC_Mesh->SetSimulatePhysics(true);

	GCC_Mesh->CrumbleActiveClusters();

	FVector Origin = GetActorLocation();

	URadialVector* RadialVector = NewObject<URadialVector>();
	RadialVector->AddToRoot();

	RadialVector->SetRadialVector(200.f, Origin);

	GCC_Mesh->ApplyPhysicsField(
		true,
		EGeometryCollectionPhysicsTypeEnum::Chaos_LinearVelocity,
		nullptr,
		RadialVector
	);

	RadialVector->RemoveFromRoot();
}

