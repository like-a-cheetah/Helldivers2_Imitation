// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AIController_Enemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Team.h"
#include "EngineUtils.h"
#include "Components/SphereComponent.h"

#include "EnemyKey.h"
#include "Tags.h"
#include "Interface/EnemyAIInterface.h"

AAIController_Enemy::AAIController_Enemy()
{
    SetupPerceptionSystem();

    //OnEnemyDetected.AddDynamic(this, &AAIController_Enemy::SetTarget);

    TeamID = FGenericTeamId(1);
}

void AAIController_Enemy::RunAI()
{
    UBlackboardComponent* BlackboardPtr = Blackboard.Get();
    if (UseBlackboard(BBAsset, BlackboardPtr))
    {
        Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

        bool RunResult = RunBehaviorTree(BTAsset);
        ensure(RunResult);
    }
}

void AAIController_Enemy::StopAI()
{
    UE_LOG(LogTemp, Log, TEXT("StopAI"));
    UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BTComponent)
    {
        BTComponent->StopTree();
    }
}

void AAIController_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CalculateDist();

    if (Target) Blackboard->SetValueAsVector(BBKEY_TARGETPOS, Target->GetActorLocation());
}

void AAIController_Enemy::BeginPlay()
{
    Super::BeginPlay();

    //for (AAIController_Enemy* OtherAI : GetTeamAIs()) //기존 다른 액터들을 저장해서 알리는 방식
    //{
    //    if (OtherAI != this)
    //    {
    //        OnEnemyDetected.AddDynamic(OtherAI, &AAIController_Enemy::SetTarget);
    //    }
    //}
}

void AAIController_Enemy::CallTeam()
{
    Blackboard->SetValueAsBool(BBKEY_CALLTEAM, true);

    FVector TraceLoc = GetPawn()->GetActorLocation();

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn());

    TArray<FHitResult> HitResults;
    GetWorld()->SweepMultiByProfile(HitResults,
        TraceLoc,
        TraceLoc,
        FQuat::Identity,
        TEXT("Enemy"),
        FCollisionShape::MakeSphere(2500),
        Params
    );
    DrawDebugSphere(GetWorld(), TraceLoc, 2500, 32, FColor::Red, false, 5.0f);

    for (auto Result : HitResults)
    {
        APawn* OtherPawn = Cast<APawn>(Result.GetActor());
        if (OtherPawn)
        {
            UE_LOG(LogTemp, Log, TEXT("%s"), *OtherPawn->GetName());
            AAIController_Enemy* OtherAIController = OtherPawn->GetController<AAIController_Enemy>();
            if (OtherAIController)
            {
                OtherAIController->ReceiveRallyCall(Target);
            }
        }
    }
}

TArray<AAIController_Enemy*> AAIController_Enemy::GetTeamAIs() const
{
    TArray<AAIController_Enemy*> Result;

    UWorld* World = GetWorld();
    if (!World) return Result;

    for (TActorIterator<AAIController_Enemy> It(World); It; ++It)
    {
        AAIController_Enemy* OtherAI = *It;

        if (OtherAI && OtherAI != this)
        {
            if (OtherAI->GetGenericTeamId() == GetGenericTeamId())
            {
                Result.Add(OtherAI);
            }
        }
    }

    return Result;
}

void AAIController_Enemy::SetupPerceptionSystem()
{
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
    SetPerceptionComponent(*PerceptionComponent);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 5500.F;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.F;
        SightConfig->PeripheralVisionAngleDegrees = 90.F;
        SightConfig->SetMaxAge(10.F);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 100.F;

        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
    }

    HearConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hear Config"));
    if (HearConfig)
    {
        HearConfig->HearingRange = 500.0f;

        HearConfig->DetectionByAffiliation.bDetectEnemies = true;
        HearConfig->DetectionByAffiliation.bDetectFriendlies = false;
        HearConfig->DetectionByAffiliation.bDetectNeutrals = false;

        PerceptionComponent->ConfigureSense(*HearConfig);
    }

    TeamConfig = CreateDefaultSubobject<UAISenseConfig_Team>(TEXT("Team Config"));
    if (TeamConfig)
    {
        PerceptionComponent->ConfigureSense(*TeamConfig);
    }

    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Enemy::ProcessStimulus);
}

void AAIController_Enemy::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    RunAI();
}

void AAIController_Enemy::ProcessStimulus(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor->ActorHasTag(TAG_PLAYER) || Target) return;

    if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>() || Stimulus.Type == UAISense::GetSenseID<UAISense_Team>())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            Target = Actor;
            Blackboard->SetValueAsObject(BBKEY_TARGET, Actor);

            IEnemyAIInterface* PawnInterface = Cast<IEnemyAIInterface>(GetPawn());
            PawnInterface->SetTarget(Actor);

            //FTimerHandle SetCallTeamVarTimeHandle;
            //GetWorld()->GetTimerManager().SetTimer(SetCallTeamVarTimeHandle, [this]() {
            //    Blackboard->SetValueAsBool(BBKEY_CALLTEAM, true);
            //    }
            //, 1.0f, false);
        }
    }
    else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            Blackboard->SetValueAsObject(BBKEY_PATROLPOS, Actor);
        }
    }
}

void AAIController_Enemy::SetTarget(AActor* Actor)
{
    Target = Actor;
    Blackboard->SetValueAsObject(BBKEY_TARGET, Actor);

    CalculateDist();

    IEnemyAIInterface* PawnInterface = Cast<IEnemyAIInterface>(GetPawn());
    PawnInterface->SetTarget(Actor);
}

void AAIController_Enemy::ReceiveRallyCall(AActor* InTarget)
{
    Blackboard->SetValueAsBool(BBKEY_CALLTEAM, true);
    SetTarget(InTarget);
}

void AAIController_Enemy::CalculateDist()
{
    if (Target && GetPawn())
    {
        FVector TargetLoc = Target->GetActorLocation();
        FVector OwnerLoc = GetPawn()->GetActorLocation();

        double dist = FVector::Distance(TargetLoc, OwnerLoc);

        Blackboard->SetValueAsFloat(BBKEY_TARGET_DIST, dist);
    }
}