// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/InteractObj.h"

#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AInteractObj::AInteractObj()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkelMeshComp->AddLocalRotation(FRotator(0.0f, 180.0f, 0.0f));
	SkelMeshComp->SetCollisionProfileName(FName(TEXT("Item")));
	RootComponent = SkelMeshComp;

	TiggerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TiggerCollision"));
	TiggerCollision->SetupAttachment(RootComponent);
	TiggerCollision->SetCollisionProfileName(FName(TEXT("ItemTrigger")));
	TiggerCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractObj::OnOverlapBegin);
	TiggerCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractObj::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat_OutLinerRef(TEXT("/Script/Engine.Material'/Game/HellDivers2/Material/Mat_OutLiner.Mat_OutLiner'"));
	if (Mat_OutLinerRef.Succeeded()) Mat_OutLiner = Mat_OutLinerRef.Object;

	InformWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	InformWidget->SetupAttachment(SkelMeshComp);

	static ConstructorHelpers::FClassFinder<UUserWidget> InformWidgetRef(TEXT("/Game/HellDivers2/UI/WBP_ObjectInformWidget.WBP_ObjectInformWidget_C"));
	if (InformWidgetRef.Class)
	{
		InformWidget->SetWidgetClass(InformWidgetRef.Class);
	}
	InformWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InformWidget->SetDrawSize(FVector2D(150.0f, 150.0f));
	InformWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InformWidget->SetHiddenInGame(true);
}

void AInteractObj::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = Cast<UUserWidget>(InformWidget->GetWidget());
	if (Widget)
	{
		UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Description")));
		if (TextBlock)
		{
			TextBlock->SetText(Description);
		}
	}
}

void AInteractObj::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	SkelMeshComp->SetOverlayMaterial(Mat_OutLiner);

	InformWidget->SetHiddenInGame(false);
}

void AInteractObj::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SkelMeshComp->SetOverlayMaterial(nullptr);

	InformWidget->SetHiddenInGame(true);
}

void AInteractObj::SetActiveOverlapEvent(bool bActive)
{
	if (bActive)
	{
		TiggerCollision->SetGenerateOverlapEvents(true);
		TiggerCollision->UpdateOverlaps();
	}
	else
	{
		SkelMeshComp->SetOverlayMaterial(nullptr);

		InformWidget->SetHiddenInGame(true);

		TiggerCollision->SetGenerateOverlapEvents(false);
		TiggerCollision->UpdateOverlaps();
	}
}

