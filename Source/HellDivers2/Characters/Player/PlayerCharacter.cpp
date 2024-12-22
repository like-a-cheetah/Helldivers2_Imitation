// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Helldivers2Instance.h"
#include "DiversPlayerController.h"
#include "Items/Item.h"
#include "CameraData.h"
#include "Items/Data/ItemData.h"
#include "Interface/GunInterface.h"
#include "Items/Grenade.h"
#include "Items/StratagemBall.h"
#include "PlayerStatComponent.h"
#include "UI/HUDWidget.h"
#include "UI/LoadOutWidget.h"
#include "UI/W_StratagemNotice.h"
#include "UI/W_StratagemCondition.h"
#include "Stratagem/StratagemData.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	//GetCapsuleComponent()->InitCapsuleSize();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/HellDivers2/Characters/Player/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)	DefaultMappingContext = InputMappingContextRef.Object;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -93.0f), FRotator(0.0f, 90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef = (TEXT("/Script/Engine.SkeletalMesh'/Game/HellDivers2/Characters/Player/models/RealModel.RealModel'"));
	if (SkeletalMeshRef.Object)	GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	HandSocketName = TEXT("attach_hand_r소켓");

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef = (TEXT("/Game/HellDivers2/Characters/Player/ABP_PlayerCharacter.ABP_PlayerCharacter_C"));
	if (SkeletalMeshRef.Object)	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	InputActionFind();
	MontageFind();
	SoundWaveFind();
	InitCameraSet();

	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));

	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::ChargeItem)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::ChargeItem)));

	ImpactWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ImpactWidget"));
	ImpactWidget->SetupAttachment(GetMesh());
	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetRef(TEXT("/Game/HellDivers2/UI/InGame/ImpactWidget.ImpactWidget_C"));
	if (AimWidgetRef.Class)
	{
		ImpactWidget->SetWidgetClass(AimWidgetRef.Class);
		ImpactWidget->SetWidgetSpace(EWidgetSpace::Screen);
		ImpactWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FClassFinder<AItem> StratagemRef(TEXT("/Game/HellDivers2/Weapons/StratagemBall.StratagemBall_C"));
	if (StratagemRef.Class)	StratagemBallClass = StratagemRef.Class;

	static ConstructorHelpers::FClassFinder<AItem> SyringeRef(TEXT("/Game/HellDivers2/Items/Syringe.Syringe_C"));
	if (SyringeRef.Class)	Syringe = SyringeRef.Class;

	static ConstructorHelpers::FClassFinder<AItem> GrenadeRef(TEXT("/Game/HellDivers2/Weapons/EXPLOSION.EXPLOSION_C"));
	if (GrenadeRef.Class)	Grenade = GrenadeRef.Class;

	HandleItem = nullptr;
	PreItem = nullptr;

	GunClassPaths.Add(TEXT("/Game/HellDivers2/Weapons/LIBERATOR.LIBERATOR_C"));
	GunClassPaths.Add(TEXT("/Game/HellDivers2/Weapons/PEACEMAKER.PEACEMAKER_C"));

	
	//스트라타젬 구현
	ConstructorHelpers::FClassFinder<AActor> TestStratagemCRef(TEXT("/Script/HellDivers2.HellPodPlayer"));
	if (TestStratagemCRef.Class) TestStratagemC = TestStratagemCRef.Class;
}

void APlayerCharacter::InputActionFind()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)			MoveAction = InputActionMoveRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)			LookAction = InputActionLookRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Run.IA_Run'"));
	if (InputActionRunRef.Object)			RunAction = InputActionRunRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_VaultRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Vault.IA_Vault'"));
	if (IA_VaultRef.Object)					IA_Vault = IA_VaultRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_HealRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Heal.IA_Heal'"));
	if (IA_HealRef.Object)					IA_Heal = IA_HealRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDivingRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Diving.IA_Diving'"));
	if (InputActionDivingRef.Object)		DivingAction = InputActionDivingRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShotRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Shot.IA_Shot'"));
	if (InputActionShotRef.Object)			ShotAction = InputActionShotRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Zoom.IA_Zoom'"));
	if (InputActionZoomRef.Object)			ZoomAction = InputActionZoomRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWeaponChangeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_WeaponChange.IA_WeaponChange'"));
	if (InputActionWeaponChangeRef.Object)	WeaponChangeAction = InputActionWeaponChangeRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTakeItemRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_TakeItem.IA_TakeItem'"));
	if (InputActionTakeItemRef.Object)		TakeItemAction = InputActionTakeItemRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Reload.IA_Reload'"));
	if (InputActionReloadRef.Object)		ReloadAction = InputActionReloadRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TakeStratagemBallRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_TakeStratagemBall.IA_TakeStratagemBall'"));
	if (IA_TakeStratagemBallRef.Object)		IA_TakeStratagemBall = IA_TakeStratagemBallRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InputStratagemBallWRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_InputStratagemBallW.IA_InputStratagemBallW'"));
	if (IA_InputStratagemBallWRef.Object)	IA_InputStratagemBallW = IA_InputStratagemBallWRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InputStratagemBallSRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_InputStratagemBallS.IA_InputStratagemBallS'"));
	if (IA_InputStratagemBallSRef.Object)	IA_InputStratagemBallS = IA_InputStratagemBallSRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InputStratagemBallDRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_InputStratagemBallD.IA_InputStratagemBallD'"));
	if (IA_InputStratagemBallDRef.Object)	IA_InputStratagemBallD = IA_InputStratagemBallDRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InputStratagemBallARef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_InputStratagemBallA.IA_InputStratagemBallA'"));
	if (IA_InputStratagemBallARef.Object)	IA_InputStratagemBallA = IA_InputStratagemBallARef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_EscapeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/UIInteract/IA_Esc.IA_Esc'"));
	if (IA_EscapeRef.Object)	IA_Escape = IA_EscapeRef.Object;
}

void APlayerCharacter::MontageFind() {
	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_Climb_LowWallRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_Climb_LowWall.MT_Climb_LowWall'"));
	if (MT_Climb_LowWallRef.Object)	MT_Climb_LowWall = MT_Climb_LowWallRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_Climb_HighWallRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_Climb_HighWall.MT_Climb_HighWall'"));
	if (MT_Climb_HighWallRef.Object)	MT_Climb_HighWall = MT_Climb_HighWallRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_Climb_JumpRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_Climb_Jump.MT_Climb_Jump'"));
	if (MT_Climb_JumpRef.Object)	MT_Climb_Jump = MT_Climb_JumpRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_Parkour_LowWallRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_Parkour_LowWall.MT_Parkour_LowWall'"));
	if (MT_Parkour_LowWallRef.Object)	MT_Parkour_LowWall = MT_Parkour_LowWallRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_Parkour_HighWallRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_Parkour_HighWall.MT_Parkour_HighWall'"));
	if (MT_Parkour_HighWallRef.Object)	MT_Parkour_HighWall = MT_Parkour_HighWallRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_GetItemRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/ItemControl/MT_GetItem.MT_GetItem'"));
	if (MT_GetItemRef.Object)	MT_GetItem = MT_GetItemRef.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_UseHealRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_UseHeal.MT_UseHeal'"));
	if (MT_UseHealRef.Object)	MT_UseHeal = MT_UseHealRef.Object;

	FName DivingStartRefPaths[4] = {
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_FrontDivingStart.MT_FrontDivingStart'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_BackDivingStart.MT_BackDivingStart'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_LeftDivingStart.MT_LeftDivingStart'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_RightDivingStart.MT_RightDivingStart'")
	};

	FName DivingEndRefPaths[4] = {
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_FrontDivingLand.MT_FrontDivingLand'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_BackDivingLand.MT_BackDivingLand'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_LeftDivingLand.MT_LeftDivingLand'"),
		TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/Diving/MT_RightDivingLand.MT_RightDivingLand'")
	};

	for (int i = 0; i < sizeof(DivingStartRefPaths) / sizeof(DivingStartRefPaths[0]); i++)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> DivingStartRef(*DivingStartRefPaths[i].ToString());
		if (DivingStartRef.Object)	MT_Divings.Add(DivingStartRef.Object);
	}

	for (int i = 0; i < sizeof(DivingEndRefPaths) / sizeof(DivingEndRefPaths[0]); i++)
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> DivingEndRef(*DivingEndRefPaths[i].ToString());
		if (DivingEndRef.Object)	MT_DivingLands.Add(DivingEndRef.Object);
	}

	static ConstructorHelpers::FObjectFinder <UAnimMontage> MT_StandUpRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_StandUp.MT_StandUp'"));
	if (MT_StandUpRef.Object)	MT_StandUp = MT_StandUpRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_StratagemInputRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/ItemControl/MT_StratagemInput.MT_StratagemInput'"));
	if (MT_StratagemInputRef.Object)	MT_StratagemInput = MT_StratagemInputRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ThrowStratagemBallRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Weapons/StratagemBall/MT_StratagemBall_Throw.MT_StratagemBall_Throw'"));
	if (MT_ThrowStratagemBallRef.Object)	MT_ThrowStratagemBall = MT_ThrowStratagemBallRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_PullingPinRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/ItemControl/MT_PullingPin.MT_PullingPin'"));
	if (MT_PullingPinRef.Object)	MT_PullingPin = MT_PullingPinRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_PlayerRebirthRef(TEXT(""));
	if (MT_PlayerRebirthRef.Object)	MT_PlayerRebirth = MT_PlayerRebirthRef.Object;

}

void APlayerCharacter::SoundWaveFind()
{
	static ConstructorHelpers::FObjectFinder<USoundWave> SW_BallArrowRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Weapons/StratagemBall/BallArrow.BallArrow'"));
	if (SW_BallArrowRef.Object)	SW_BallArrow = SW_BallArrowRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_BallLoopEnterRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Weapons/StratagemBall/BallLoopEnter.BallLoopEnter'"));
	if (SW_BallLoopEnterRef.Object)	SW_BallLoopEnter = SW_BallLoopEnterRef.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_DiveLandRef(TEXT("/Script/Engine.SoundWave'/Game/HellDivers2/Characters/Player/Sounds/0x4d19345d84e25beb_45__10237972_.0x4d19345d84e25beb_45__10237972_'"));
	if (SW_DiveLandRef.Object)	SW_DiveLand = SW_DiveLandRef.Object;
}

void APlayerCharacter::InitCameraSet()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 15.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 10.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetWorldRotation(FRotator(-5.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UCameraData> BasicCameraDataRef(TEXT("/Script/HellDivers2.CameraData'/Game/HellDivers2/Characters/Player/CameraData/Basic.Basic'"));
	if (BasicCameraDataRef.Object)
	{
		CameraDataManager.Add(0, BasicCameraDataRef.Object);
		CameraDataManager.Add(2, BasicCameraDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCameraData> ThirdAimingCameraDataRef(TEXT("/Script/HellDivers2.CameraData'/Game/HellDivers2/Characters/Player/CameraData/ThirdAimingCameraData.ThirdAimingCameraData'"));
	if (ThirdAimingCameraDataRef.Object)	CameraDataManager.Add(1, ThirdAimingCameraDataRef.Object);

	static ConstructorHelpers::FObjectFinder<UCameraData> ProneThirdAimingRef(TEXT("/Script/HellDivers2.CameraData'/Game/HellDivers2/Characters/Player/CameraData/ProneThirdAiming.ProneThirdAiming'"));
	if (ProneThirdAimingRef.Object)	CameraDataManager.Add(3, ProneThirdAimingRef.Object);

	CurrentPose = EPose::Stand;
	SetCameraData(CameraDataManager[0]);

	CameraBoom->bDoCollisionTest = true;

	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCaptureComp->SetupAttachment(RootComponent);
	SceneCaptureComp->SetWorldRotation(FRotator(0.0f, 170.0f, 30.0f));
	SceneCaptureComp->SetRelativeLocation(FVector(170.0, -30.0, 0));
	SceneCaptureComp->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComp->OrthoWidth = 170.0f;

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> CharCaptureTextureRef(TEXT("/Game/HellDivers2/UI/Source/LoadOut/RT_CameraView.RT_CameraView"));
	if(CharCaptureTextureRef.Succeeded()) SceneCaptureComp->TextureTarget = CharCaptureTextureRef.Object;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetStratagemFromGInst();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	DiversController = CastChecked<ADiversPlayerController>(NewController);
	if (!DiversController)
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);

	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);
	GetCharacterMovement()->GravityScale = 0.0f;
	CameraBoom->TargetArmLength = 2000.0f;
	CameraBoom->bDoCollisionTest = false;
	
	SceneCaptureComp->ShowOnlyActorComponents(this, true);

	SetCameraData(CameraDataManager[0]);

	//SuccessStratagemIndex = -1;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(DiversController->GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultMappingContext, 0);

	SetStratagemFromGInst();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckDiveLanding();

	SetImpactPoint();
	

	//if (CurrentPose == EPose::Prone && bRightButton)
	//{
	//	if(HandleItem)
	//		FollowCamera->SetWorldRotation(HandleItem->GetActorQuat());
	//}


}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AItem* DroppedItem = Cast<AItem>(OtherActor);
	if (DroppedItem)
	{
		GroundedItems.Add(DroppedItem);
	}
}

void APlayerCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AItem* temp = Cast<AItem>(OtherActor);
	GroundedItems.Remove(temp);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::MoveEnd);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::Run);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::Run);
	EnhancedInputComponent->BindAction(DivingAction, ETriggerEvent::Started, this, &APlayerCharacter::Diving);
	EnhancedInputComponent->BindAction(IA_Vault, ETriggerEvent::Triggered, this, &APlayerCharacter::VaultObstacles);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::VaultObstacles);
	EnhancedInputComponent->BindAction(IA_Heal, ETriggerEvent::Started, this, &APlayerCharacter::Heal);

	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftButtonStarted);
	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftButtonTriggered);
	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Completed, this, &APlayerCharacter::LeftButtonEnd);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &APlayerCharacter::Zoom);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APlayerCharacter::Zoom);
	EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeWeaponAction);
	EnhancedInputComponent->BindAction(TakeItemAction, ETriggerEvent::Started, this, &APlayerCharacter::TakeItem);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
	EnhancedInputComponent->BindAction(IA_TakeStratagemBall, ETriggerEvent::Started, this, &APlayerCharacter::TakeStratagemBall);
	EnhancedInputComponent->BindAction(IA_TakeStratagemBall, ETriggerEvent::Completed, this, &APlayerCharacter::TakeStratagemBall);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallW, ETriggerEvent::Started, this, &APlayerCharacter::InputStratagemBall);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallS, ETriggerEvent::Started, this, &APlayerCharacter::InputStratagemBall);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallD, ETriggerEvent::Started, this, &APlayerCharacter::InputStratagemBall);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallA, ETriggerEvent::Started, this, &APlayerCharacter::InputStratagemBall);

	EnhancedInputComponent->BindAction(IA_Escape, ETriggerEvent::Started, this, &APlayerCharacter::Esc);
}

void APlayerCharacter::ChangeWeaponAction(const FInputActionValue& Value)
{
	const int32 WeaponIndex = Value.GetMagnitude();

	if (WeaponIndex == int32(EItemType::Projectile))
	{
		TakeOutGreade();
	}
	else
	{
		SwapWeapon(EItemType(WeaponIndex));
	}
}

void APlayerCharacter::SwapWeapon(EItemType NextItemType)
{
	if (!HandleItem || HandleItem->GetItemType() == NextItemType || !Weapons.Find(NextItemType))
		return;

	PreItem = HandleItem;
	PlayAnimMontage(HandleItem->GetInsertMontage());

	FOnMontageEnded MT_EndDelegate;
	MT_EndDelegate.BindLambda([this, NextItemType](UAnimMontage* Montage, bool bInterrupted)
		{
			if (HandleItem->GetItemType() == EItemType::Projectile)
			{
				HandleItem->Destroy();
			}
			else
			{
				AttachToSocket(HandleItem, HandleItem->GetSocketName());
			}

			HandleItem = *(Weapons.Find(NextItemType));

			AttachToSocket(HandleItem, HandSocketName);
			PlayAnimMontage(HandleItem->GetTakeOutMontage());

			Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
		}
	);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndDelegate);
}

void APlayerCharacter::TakeOutGreade()
{
	if (GrenadeNum <= 0)
		return;

	if (HandleItem)
	{
		PlayAnimMontage(HandleItem->GetInsertMontage());

		FOnMontageEnded MT_EndDelegate;
		MT_EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				AttachToSocket(HandleItem, HandleItem->GetSocketName());
				PreItem = HandleItem;

				HandleItem = GetWorld()->SpawnActor<AItem>(Grenade);

				AttachToSocket(HandleItem, HandSocketName);
				PlayAnimMontage(HandleItem->GetTakeOutMontage());
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndDelegate);
	}
	else
	{
		HandleItem = GetWorld()->SpawnActor<AItem>(Grenade);

		AttachToSocket(HandleItem, HandSocketName);
		PlayAnimMontage(HandleItem->GetTakeOutMontage());
	}
}

void APlayerCharacter::ThrowItem()
{
	FRotator Direction = GetControlRotation();
	Direction.Pitch += 10.0f;

	HandleItem->GetSkelMeshComp()->SetSimulatePhysics(true);
	HandleItem->GetSkelMeshComp()->AddImpulse(Direction.Vector() * 2000.0f);

	SetLookingForward(true);

	GetMesh()->GetAnimInstance()->Montage_Play(MT_ThrowStratagemBall, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);

	HandleItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

EItemType APlayerCharacter::GetCurrentItemType()
{
	if (HandleItem)
		return HandleItem->GetItemType();
	return EItemType::None;
}

void APlayerCharacter::GetCurrentZ()
{
	FVector Temp = GetMesh()->GetSocketLocation(TEXT("StingrayEntityRoot"));
	Temp.Z = Height;
	VaultDestinationPos = Temp;
}

void APlayerCharacter::SetupHUDWidget(UUserWidget* InHUDWidget)
{
	UHUDWidget* HUDWidget = Cast<UHUDWidget>(InHUDWidget);
	if (HUDWidget)
	{
		HUDWidget->UpdateHpBar(Stat->GetCurHp());
		HUDWidget->UpdateStat(GrenadeNum, HandleItem ? GunsMagazineN[HandleItem->GetItemType()] : 0, SyringeNum);

		Stat->OnHpChanged.AddUObject(HUDWidget, &UHUDWidget::UpdateHpBar);
		Stat->OnItemChanged.AddUObject(HUDWidget, &UHUDWidget::UpdateStat);
		Stat->OnGrenadeChanged.AddUObject(HUDWidget, &UHUDWidget::UpdateGrenadeN);

		//OnActiveStratagem.BindUObject(HUDWidget, &UHUDWidget::ActiveStratagemWidget);
	}

	ULoadOutWidget* LoadOutWidget = Cast<ULoadOutWidget>(InHUDWidget);
	if (LoadOutWidget)
	{
		// 플레이어 레벨에 따라서 HUD에서 보여줄 버튼의 가짓수 결정하는 방식 만들기?
		// LoadOutWidget->SetValiableStratagems(ValiableStratagemsData);

		OnCloseStratagemSettingWidget.BindUObject(LoadOutWidget, &ULoadOutWidget::ExitSettingStratagem);
	}
}

void APlayerCharacter::SetStratagemFromGInst()
{
	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName == "InGameTestmap")
	{
		UHelldivers2Instance* GInst = Cast<UHelldivers2Instance>(GetGameInstance());
		if (GInst)
		{
			Stratagems = GInst->GetTempStratagemsD();
			OnStratagemSet.Execute(Stratagems);
		}
	}
}

void APlayerCharacter::PlayerRebirth()
{
	UE_LOG(LogTemp, Log, TEXT("Rebirth"));
	PlayAnimMontage(MT_PlayerRebirth);
}

void APlayerCharacter::SetPlayerStratagem(UStratagemData* SData)
{
	Stratagems.Add(SData);
}

void APlayerCharacter::SetStratagemConditionWidget(UUserWidget* InStratagemNoticeWidget)
{
	UW_StratagemCondition* W_StratagemCondition = Cast<UW_StratagemCondition>(InStratagemNoticeWidget);
	if (W_StratagemCondition)
	{
		FOnShowConditionWidget ShowConditionWidgetDelegate;
		ShowConditionWidgetDelegate.BindUObject(W_StratagemCondition, &UW_StratagemCondition::ShowWidget);
		OnShowConditionDelegates.Add(ShowConditionWidgetDelegate);

		//FOnSetActiveW SetInactiveWDelegate;
		//SetInactiveWDelegate.BindUObject(W_StratagemCondition, &UW_StratagemCondition::SetInactiveW);
		//OnSetActiveWDelegates.Add(SetInactiveWDelegate);
	}
}

void APlayerCharacter::SetStratagemsNoticeWidget(UUserWidget* InStratagemNoticeWidget)
{
	UW_StratagemNotice* W_StratagemNotice = Cast<UW_StratagemNotice>(InStratagemNoticeWidget);
	if (W_StratagemNotice)
	{
		OnStratagemSet.BindUObject(W_StratagemNotice, &UW_StratagemNotice::SetStratagemWidget);
	}
}

void APlayerCharacter::LoadLevel(FName InLevelName)
{
	UE_LOG(LogTemp, Log, TEXT("Load %s"), *InLevelName.ToString());
	UGameplayStatics::OpenLevel(GetWorld(), InLevelName);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (CurrentPose == EPose::Prone)
	{
		SetLookingForward(true);
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::MoveEnd(const FInputActionValue& Value)
{
	if (CurrentPose == EPose::Prone)
	{
		SetLookingForward(false);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	rotateValue.X += LookAxisVector.X;
	rotateValue.Y += LookAxisVector.Y;
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::Run(const FInputActionValue& Value)
{
	LShiftPressed = Value.Get<bool>();

	if (CurrentPose == EPose::Prone)
	{
		PlayAnimMontage(MT_StandUp);
		SetLookingForward(false);

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				CurrentPose = EPose::Stand;
				SetSpeedFromCondition();
				SetLookingForward(false);
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);
		
		return;
	}

	SetSpeedFromCondition();
}

void APlayerCharacter::LeftButtonStarted()
{
	if (!HandleItem)
		return;

	IGunInterface* GunInterface = Cast<IGunInterface>(HandleItem);
	if (GunInterface)
	{
		if (!GunInterface->IsBurst())
		{
			GunInterface->Shot();
		}
	}
	else if (HandleItem->GetItemType() == EItemType::Projectile)
	{
		PlayAnimMontage(MT_PullingPin);
		Cast<AGrenade>(HandleItem)->PullingPin();
		bPullingPin = true;

		GrenadeNum--;

		if (Stat)	Stat->OnGrenadeChanged.Broadcast(GrenadeNum);
	}
	else if (HandleItem->GetItemType() == EItemType::StratagemBall) {
		HandleItem->GetSkelMeshComp()->SetNotifyRigidBodyCollision(true);
		ThrowItem();

		HandleItem = PreItem;
		FOnMontageEnded MontageEnded;
		MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool Interrupted)
			{
				if (!bRightButton)
					SetLookingForward(false);

				AttachToSocket(HandleItem, HandSocketName);
				PlayAnimMontage(HandleItem->GetTakeOutMontage());
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded);

		bSucceededStratagem = false;
	}
}

void APlayerCharacter::LeftButtonTriggered()
{
	IGunInterface* GunInterface = Cast<IGunInterface>(HandleItem);
	if (GunInterface)
	{
		if (GunInterface->IsBurst())
		{
			GunInterface->Shot();
			SetLookingForward(true);
		}
	}
}

void APlayerCharacter::LeftButtonEnd()
{
	if (HandleItem && HandleItem->GetItemType() == EItemType::Projectile)
	{
		bPullingPin = false;

		ThrowItem();

		if (GrenadeNum > 0)
		{
			FOnMontageEnded MontageEnded;
			MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool Interrupted)
				{
					if (!bRightButton)
						SetLookingForward(false);

					HandleItem = GetWorld()->SpawnActor<AItem>(Grenade);

					AttachToSocket(HandleItem, HandSocketName);
					PlayAnimMontage(HandleItem->GetTakeOutMontage());
				}
			);
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded);
		}
		else
		{
			HandleItem = nullptr;

			if (PreItem->GetItemType() != EItemType::Projectile)
			{
				FOnMontageEnded MontageEnded;
				MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool Interrupted)
					{
						if (!bRightButton)
							SetLookingForward(false);

						HandleItem = PreItem;

						AttachToSocket(HandleItem, HandSocketName);
						PlayAnimMontage(HandleItem->GetTakeOutMontage());
					}
				);
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded);
			}
			else
			{
				FOnMontageEnded MontageEnded;
				MontageEnded.BindLambda([this](UAnimMontage* AnimMontage, bool Interrupted)
					{
						if (!bRightButton)
							SetLookingForward(false);
					}
				);
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded);
			}
		}
	}
	else
	{
		if (!bRightButton)
			SetLookingForward(false);
	}
}

void APlayerCharacter::Recoil(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void APlayerCharacter::Zoom(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		bRightButton = true;

		UCameraData* NewCameraData;
		NewCameraData = CameraDataManager[1];
		SetLookingForward(true);
		if (CurrentPose != EPose::Prone)
		{
			//NewCameraData = CameraDataManager[1];
			SetLookingForward(true);
		}
		else
		{
			//NewCameraData = CameraDataManager[3];
			SetLookingForward(false);
		}

		CameraBoom->TargetArmLength = NewCameraData->TargetArmLength;
		CameraBoom->SocketOffset = NewCameraData->SocketOffset;

		FollowCamera->SetRelativeRotation(NewCameraData->Rot);
	}
	else
	{
		bRightButton = false;

		UCameraData* NewCameraData;
		if (CurrentPose != EPose::Prone)
		{
			NewCameraData = CameraDataManager[0];
		}
		else
		{
			NewCameraData = CameraDataManager[2];
		}

		CameraBoom->TargetArmLength = NewCameraData->TargetArmLength;
		CameraBoom->SocketOffset = NewCameraData->SocketOffset;

		FollowCamera->SetRelativeRotation(NewCameraData->Rot);

		SetLookingForward(false);
	}
}

void APlayerCharacter::Diving()	//상하좌우 순서대로 0, 1, 2, 3
{
	if (CurrentPose == EPose::Stand)
	{
		FVector Direct = GetLastMovementInputVector().GetSafeNormal();

		FVector DivingVel = Direct * 1000.0f;
		DivingVel.Z += 250.0f;

		float ForwardDot = FVector::DotProduct(Direct, FollowCamera->GetForwardVector());
		float BackDot = FVector::DotProduct(Direct, -FollowCamera->GetForwardVector());
		float RightDot = FVector::DotProduct(Direct, FollowCamera->GetRightVector());
		float LeftDot = FVector::DotProduct(Direct, -FollowCamera->GetRightVector());

		LaunchCharacter(DivingVel, true, true);

		float MaxDot = ForwardDot;
		int32 ClosestDirection = 0;
		FRotator DiveStartRot = GetActorRotation();
		DiveStartRot.Yaw = Direct.Rotation().Yaw;

		if (BackDot > MaxDot)
		{
			MaxDot = BackDot;
			ClosestDirection = 1;
		}
		if (LeftDot > MaxDot)
		{
			MaxDot = LeftDot;
			ClosestDirection = 2;
		}
		if (RightDot > MaxDot)
		{
			MaxDot = RightDot;
			ClosestDirection = 3;
		}

		SetActorRotation(DiveStartRot);
		PlayAnimMontage(MT_Divings[ClosestDirection]);

		DisableInput(DiversController);		
	}
	else if (CurrentPose == EPose::Prone)
	{
		PlayAnimMontage(MT_StandUp);
		CurrentPose = EPose::Stand;
		SetLookingForward(false);

		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				CurrentPose = EPose::Stand;
				GetCharacterMovement()->MaxWalkSpeed = 700.0f;
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);
	}
}

void APlayerCharacter::CheckDiveLanding()
{
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling && CurrentPose != EPose::Prone)
	{
		UAnimMontage* CurrentMontage = GetCurrentMontage();
		for (int i = 0; i < MT_Divings.Num(); i++)
		{
			if (CurrentMontage == MT_Divings[i])
			{
				PlayAnimMontage(MT_DivingLands[i]);
				FOnMontageEnded OnMontageEnded;
				OnMontageEnded.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
					{
						EnableInput(DiversController);
					});
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);

				UGameplayStatics::PlaySoundAtLocation(this, SW_DiveLand, GetActorLocation());

				GetCharacterMovement()->MaxWalkSpeed = 500.0f;
				CurrentPose = EPose::Prone;

				return;
			}
		}
	}
}

void APlayerCharacter::SetImpactPoint()
{
	if (HandleItem)
	{
		if ((HandleItem->GetItemType() == EItemType::Main || HandleItem->GetItemType() == EItemType::Pistol || HandleItem->GetItemType() == EItemType::Stratagem)
			&& bRightButton)
		{
			ImpactWidget->SetHiddenInGame(false);

			IGunInterface* HandleGun = Cast<IGunInterface>(HandleItem);
			ImpactWidget->SetWorldLocation(HandleGun->GetImpactPoint());
		}
		else
		{
			ImpactWidget->SetHiddenInGame(true);
		}
	}
	else
	{
		ImpactWidget->SetHiddenInGame(true);
	}
}

void APlayerCharacter::TakeStratagemBall(const FInputActionValue& Value)
{
	bActivatedStratagemBall = Value.Get<bool>();
	if (bSucceededStratagem)
		return;

	if (Value.Get<bool>())
	{
		MacroIndex = 0;
		for (int i = 0; i < Stratagems.Num(); i++)
		{
			OnShowConditionDelegates[i].Execute(true);
			Stratagems[i]->SetbActive(true);
		}
		//OnActiveStratagem.Execute(true);
		// 
		//for(int i=0; i<Stratagems.Num(); i++)
			//ActivatedMacros.Add(Stratagems[i]->Macro);

		AItem* StratagemBall = GetWorld()->SpawnActor<AItem>(StratagemBallClass);

		if (HandleItem)
		{
			PlayAnimMontage(HandleItem->GetInsertMontage());
			PreItem = HandleItem;

			FOnMontageEnded MT_EndedDelegate;
			MT_EndedDelegate.BindLambda([this, StratagemBall](UAnimMontage* Montage, bool bInterrupted)
				{
					if (PreItem->GetItemType() == EItemType::Projectile)
						PreItem->Destroy();
					else AttachToSocket(PreItem, PreItem->GetSocketName());

					HandleItem = StratagemBall;
					AttachToSocket(HandleItem, HandSocketName);
					PlayAnimMontage(HandleItem->GetTakeOutMontage());
				}
			);
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndedDelegate);
		}
		else
		{
			HandleItem = StratagemBall;
			HandleItem->SetActorHiddenInGame(false);
			AttachToSocket(HandleItem, HandSocketName);
			PlayAnimMontage(HandleItem->GetTakeOutMontage());
		}

		bSucceededStratagem = false;

		SetLookingForward(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else if (!Value.Get<bool>())
	{
		if (HandleItem)
		{
			if (HandleItem->GetItemType() == EItemType::StratagemBall)
			{
				for (int i = 0; i < Stratagems.Num(); i++) OnShowConditionDelegates[i].Execute(false);
				//OnActiveStratagem.Execute(false);
				PlayAnimMontage(HandleItem->GetInsertMontage());

				HandleItem->Destroy();
				HandleItem = PreItem;
				if (PreItem)
				{
					FOnMontageEnded MT_EndedDelegate;
					MT_EndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
						{
							GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
							SetLookingForward(false);

							AttachToSocket(HandleItem, HandSocketName);
							PlayAnimMontage(HandleItem->GetTakeOutMontage());
						}
					);
					GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndedDelegate);
				}
				else
				{
					FOnMontageEnded MT_EndedDelegate;
					MT_EndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
						{
							GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
							SetLookingForward(false);
						}
					);
					GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndedDelegate);
				}
			}
			else
			{
				if (GetCurrentMontage() == HandleItem->GetInsertMontage())
				{
					FOnMontageEnded MT_EndedDelegate;
					MT_EndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
						{
							PlayAnimMontage(HandleItem->GetTakeOutMontage());
							GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
						}
					);
					GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndedDelegate);
				}
			}
		}
	}
}

void APlayerCharacter::InputStratagemBall(const FInputActionValue& Value)
{
	if (bSucceededStratagem || !bActivatedStratagemBall)
		return;

	PlayAnimMontage(MT_StratagemInput);
	UGameplayStatics::PlaySoundAtLocation(this, SW_BallArrow, GetActorLocation());

	const uint8 InputMacro = Value.Get<float>();

	bool bAllInactive = true;

	for (auto Stratagem : Stratagems)
	{
		if (!Stratagem->IsActive() || MacroIndex >= Stratagem->Macro.Num()) 
		{
			if (Stratagem->IsActive())
				Stratagem->SetbActive(false);
			continue;
		}
		else {
			const uint8 MacroKey = Stratagem->Macro[MacroIndex];
			if (MacroKey == InputMacro) 
			{
				bAllInactive = false;

				const uint8 MacroN = Stratagem->Macro.Num();
				if (MacroIndex == MacroN - 1)
				{
					bSucceededStratagem = true;
								
					UGameplayStatics::PlaySoundAtLocation(this, SW_BallLoopEnter, GetActorLocation());
								
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					SetLookingForward(false);
								
					//보류. 이방법 좀 더 생각하기, 이 액터 스폰할 때 TSubclass로 만들고 AItem 타입 변수로 받는데
					Cast<AStratagemBall>(HandleItem)->SetStratagem(TestStratagemC);

					for (int i = 0; i < Stratagems.Num(); i++)
					{
						if (Stratagems[i] != Stratagem) OnShowConditionDelegates[i].Execute(false);
					}
								
					return;
				}
			}
			else Stratagem->SetbActive(false);

		}
	}

	if(!bAllInactive)
		MacroIndex++;
	else
	{
		MacroIndex = 0;

		for (auto Stratagem : Stratagems) Stratagem->SetbActive(true);
	}

	//for (int i = 0; i < ActivatedMacros.Num(); ) 
	//{
	//	if (MacroIndex >= ActivatedMacros[i].Num() || InputValue != ActivatedMacros[i][MacroIndex])
	//	{
	//		ActivatedMacros.RemoveAt(i);
	//		Stratagems[i]->SetbActive(false);
	//		//OnSetActiveWDelegates[i].Execute(false);
	//	}
	//	else 
	//	{
	//		if (ActivatedMacros[i].Num() - 1 == MacroIndex) 
	//		{
	//			bSucceededStratagem = true;
	//			
	//			UGameplayStatics::PlaySoundAtLocation(this, SW_BallLoopEnter, GetActorLocation());
	//			
	//			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//			SetLookingForward(false);
	//			
	//			//보류. 이방법 좀 더 생각하기, 이 액터 스폰할 때 TSubclass로 만들고 AItem 타입 변수로 받는데
	//			Cast<AStratagemBall>(HandleItem)->SetStratagem(TestStratagemC);
	//			
	//			return;
	//		}
	//		i++;
	//	}
	//	//i 번 위젯 업데이트 하기
	//}
	//
	//MacroIndex++;

	//bool temp = ActivatedMacros.IsEmpty();
	//if (ActivatedMacros.IsEmpty()) 
	//{
	//	for (int i = 0; i < Stratagems.Num(); i++) 
	//	{
	//		ActivatedMacros.Add(Stratagems[i]->Macro);
	//		Stratagems[i]->SetbActive(true);
	//		//OnSetActiveWDelegates[i].Execute(true);
	//	}
	//	//위젯 업데이트 하기
	//	MacroIndex = 0;
	//}
}

void APlayerCharacter::Esc()
{
	OnCloseStratagemSettingWidget.Execute();
}

void APlayerCharacter::SetSpeedFromCondition()
{
	if (CurrentPose == EPose::Stand) {
		if(LShiftPressed)
			GetCharacterMovement()->MaxWalkSpeed = 700.0f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
	else if (CurrentPose == EPose::Prone) {
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	else if (CurrentPose == EPose::Crawl) {
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

bool APlayerCharacter::TryGetObstacleHeight(float& WallHeight)
{
	float PlayerHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 2.212502;

	FVector Start = GetActorLocation();
	float GroundHeight = Start.Z - PlayerHeight;
	Start.Z = GroundHeight + 50.0f;

	FVector End = Start + (GetActorForwardVector() * 80.0f);

	FHitResult ForwardHitResult;
	GetWorld()->LineTraceSingleByObjectType(ForwardHitResult, Start, End, ECC_WorldStatic);

	if (ForwardHitResult.bBlockingHit)
	{
		FHitResult TopHitResult;
		FVector AdjustedForwardImpactPoint = ForwardHitResult.ImpactPoint + (GetActorForwardVector() * 20.0f);
		FVector ForwardHeightPos = AdjustedForwardImpactPoint + FVector(0.0f, 0.0f, 200.0f);

		GetWorld()->LineTraceSingleByObjectType(TopHitResult, ForwardHeightPos, AdjustedForwardImpactPoint, ECC_WorldStatic);

		WallHeight = TopHitResult.ImpactPoint.Z - GroundHeight;
		if (TopHitResult.bBlockingHit && WallHeight <= 180.0f)
		{
			FVector ParkourDestination = TopHitResult.ImpactPoint + (GetActorForwardVector() * 60.0f);
			ParkourDestination.Z = TopHitResult.ImpactPoint.Z - 30.0f;
			FVector TicknessHeightPos = ParkourDestination;
			TicknessHeightPos.Z = ForwardHeightPos.Z;

			FHitResult TicknessResult;
			GetWorld()->LineTraceSingleByObjectType(TicknessResult, TicknessHeightPos, ParkourDestination, ECC_WorldStatic);

			if (TicknessResult.bBlockingHit)
			{
				bWalkableWall = true;
				Height = TopHitResult.ImpactPoint.Z + PlayerHeight;
			}
			else
			{
				bWalkableWall = false;
				Height = GetActorLocation().Z;
			}
			return true;
		}
	}
	return false;
}

void APlayerCharacter::SetLookingForward(bool bLookForward)
{
	if (bLookForward)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}

void APlayerCharacter::SetCameraData(const UCameraData* CameraData)
{
	CameraBoom->TargetArmLength = CameraData->TargetArmLength;
	CameraBoom->SocketOffset = CameraData->SocketOffset;


	FollowCamera->SetWorldRotation(CameraData->Rot);
}

void APlayerCharacter::TakeItem(const FInputActionValue& Value)
{
	if (!GroundedItems.IsEmpty())
	{
		PlayAnimMontage(MT_GetItem);

		AItem* Item = GroundedItems.Pop();
		TakeItemActions[(uint8)Item->GetItemType()].ItemDelegate.ExecuteIfBound(Item);
	}
}

void APlayerCharacter::VaultObstacles(const FInputActionValue& Value)
{
	if (CurrentPose != EPose::Stand)
		return;

	float WallHeight;
	if (TryGetObstacleHeight(WallHeight) && GetCharacterMovement()->IsMovingOnGround() && !bVault)
	{
		bVault = true;
		
		CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName(TEXT("StingrayEntityRoot")));

		DisableInput(DiversController);

		if (bWalkableWall)
		{
			if (WallHeight <= 100.0f)
			{
				PlayAnimMontage(MT_Climb_LowWall);
			}
			else
			{
				PlayAnimMontage(MT_Climb_HighWall);
			}
		}
		else
		{
			if (WallHeight <= 100.0f)
			{
				PlayAnimMontage(MT_Parkour_LowWall);
			}
			else
			{
				PlayAnimMontage(MT_Parkour_HighWall);
			}
		}
		FOnMontageEnded MT_EndDelegate;
		MT_EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				EnableInput(DiversController);
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				bVault = false;
				bWalkableWall = false;
				SetActorLocation(VaultDestinationPos);

				LShiftPressed = false;
				SetSpeedFromCondition();

				CameraBoom->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndDelegate);
	}
}

void APlayerCharacter::Heal(const FInputActionValue& Value)
{
	if (SyringeNum)
	{
		if (HandleItem)
			HandleItem->SetActorHiddenInGame(true);

		AItem* SpawnedSyringe = GetWorld()->SpawnActor<AItem>(Syringe);
		AttachToSocket(SpawnedSyringe, HandSocketName);

		PlayAnimMontage(MT_UseHeal);
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this, SpawnedSyringe](UAnimMontage* Montage, bool bInterrupted)
			{
				SpawnedSyringe->Destroy();
				HandleItem->SetActorHiddenInGame(false);

				Stat->SetHp(Stat->GetMaxHp());
			});
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);

		SyringeNum--;

		if (Stat)
			Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
	}
}

void APlayerCharacter::EquipWeapon(AItem* Item)
{
	if (Item)
	{
		EItemType NewItemType = Item->GetItemType();

		if (Weapons.Find(NewItemType))
		{
			AItem* InventoryItem = *Weapons.Find(NewItemType);
			Weapons.Remove(NewItemType);
			InventoryItem->SetOwner(nullptr);
			InventoryItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Item->GetSkelMeshComp()->SetSimulatePhysics(true);
			if (HandleItem == InventoryItem)
				HandleItem = nullptr;
		}

		if (!HandleItem)
		{
			HandleItem = Item;
			AttachToSocket(Item, HandSocketName);
		}
		else
		{
			AttachToSocket(Item, Item->GetSocketName());
		}

		Weapons.Add(NewItemType, Item);
		Item->SetOwner(this);
		Item->GetSkelMeshComp()->SetGenerateOverlapEvents(false);
	}
}

void APlayerCharacter::ChargeItem(AItem* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("소비 아이템은 보급으로만 충전 가능"));

	Item->Destroy();
}

void APlayerCharacter::ChargeConsumedItem()
{
	SyringeNum = 4;
	GrenadeNum = 4;
	GunsMagazineN[EItemType::Main] = 4;
	GunsMagazineN[EItemType::Pistol] = 4;
	GunsMagazineN[EItemType::Stratagem] = 4;
}

void APlayerCharacter::BeginWeaponEquip()
{
	GunsMagazineN.Add(EItemType::Main, 4);
	GunsMagazineN.Add(EItemType::Pistol, 4);
	GunsMagazineN.Add(EItemType::Stratagem, 4);

	SyringeNum = 4;
	GrenadeNum = 4;

	for (int i = 0; i < 2; i++)
	{
		FString ClassPath = GunClassPaths[i];
		UClass* WeaponClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *ClassPath));
		AItem* Weapon = GetWorld()->SpawnActor<AItem>(WeaponClass);
		EquipWeapon(Weapon);
	}

	if (Stat)
		Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
}

void APlayerCharacter::AttachToSocket(AItem* Item, FName SocketName)
{
	FAttachmentTransformRules AttachmentTransformRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Item->GetSkelMeshComp()->SetSimulatePhysics(false);
	Item->AttachToComponent(GetMesh(), AttachmentTransformRule, SocketName);
}

void APlayerCharacter::Reload()
{
	IGunInterface* GunInterface = Cast<IGunInterface>(HandleItem);
	if (GunInterface)
	{
		if(GunInterface->IsCompleteReload() && GunsMagazineN[HandleItem->GetItemType()] > 0) {
			PlayAnimMontage(GunInterface->GetReloadMontage());
			GunInterface->PlayReloadMontage();
			GunsMagazineN[HandleItem->GetItemType()]--;

			if(Stat)
				Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
		}
	}
}