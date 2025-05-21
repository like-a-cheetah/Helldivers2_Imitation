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
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "Tags.h"
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
#include "Interface/ObjectInterface.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	bActiveLookAction = true;
	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	//GetCapsuleComponent()->InitCapsuleSize();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
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

	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef = (TEXT("/Game/HellDivers2/Characters/Player/ABP_PlayerCharacter.ABP_PlayerCharacter_C"));
	//if (AnimInstanceClassRef.Class)	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	InputActionFind();
	MontageFind();
	SoundWaveFind();
	InitCameraSet();

	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
	Stat->OnHpZero.AddDynamic(this, &APlayerCharacter::Die);

	TakeItemActions.Add(EItemType::AmmoBox, FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::ChargeItem)));

	TakeItemActions.Add(EItemType::Pistol, FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemActions.Add(EItemType::Main, FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));
	TakeItemActions.Add(EItemType::Stratagem, FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APlayerCharacter::EquipWeapon)));

	//ImpactWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ImpactWidget"));
	//ImpactWidget->SetupAttachment(GetMesh());
	//static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetRef(TEXT("/Game/HellDivers2/UI/InGame/ImpactWidget.ImpactWidget_C"));
	//if (AimWidgetRef.Class)
	//{
	//	ImpactWidget->SetWidgetClass(AimWidgetRef.Class);
	//	ImpactWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//	ImpactWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	ImpactWidget->SetGenerateOverlapEvents(false);
	//}

	static ConstructorHelpers::FClassFinder<AItem> StratagemRef(TEXT("/Game/HellDivers2/Weapons/BP_StratagemBall.BP_StratagemBall_C"));
	if (StratagemRef.Class)	StratagemBallC = StratagemRef.Class;

	static ConstructorHelpers::FClassFinder<AItem> SyringeRef(TEXT("/Game/HellDivers2/Items/Syringe.Syringe_C"));
	if (SyringeRef.Class)	SyringeC = SyringeRef.Class;

	static ConstructorHelpers::FClassFinder<AItem> GrenadeRef(TEXT("/Game/HellDivers2/Weapons/EXPLOSION.EXPLOSION_C"));
	if (GrenadeRef.Class)	GrenadeC = GrenadeRef.Class;

	HandleItem = nullptr;
	PreItem = nullptr;

	GunClassPaths.Add(TEXT("/Game/HellDivers2/Weapons/LIBERATOR.LIBERATOR_C"));
	GunClassPaths.Add(TEXT("/Game/HellDivers2/Weapons/PEACEMAKER.PEACEMAKER_C"));

	//실제 게임 시뮬시 주석
	//스트라타젬 구현 데모
	static ConstructorHelpers::FObjectFinder<UStratagemData> ResupplyStratagemDataRef(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Resupply/Resupply.Resupply'"));
	if (ResupplyStratagemDataRef.Object) Stratagems.Add(ResupplyStratagemDataRef.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> OrbitStratagemDataRef(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/120mm_HE_Barrage/Orbital_120mm_HE_Barrage_Data.Orbital_120mm_HE_Barrage_Data'"));
	if (OrbitStratagemDataRef.Object) Stratagems.Add(OrbitStratagemDataRef.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> dsadsaDataRef(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/Precision_Strike_Data/Orbital_Precision_Strike_Data.Orbital_Precision_Strike_Data'"));
	if (dsadsaDataRef.Object) Stratagems.Add(dsadsaDataRef.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> asd(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/Gatling_Barrage/Orbital_Gatling_Barrage_Data.Orbital_Gatling_Barrage_Data'"));
	if (asd.Object) Stratagems.Add(asd.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> a(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/380mm_HE_Barrage/Orbital_380mm_HE_Barrage_Data.Orbital_380mm_HE_Barrage_Data'"));
	if (a.Object) Stratagems.Add(a.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> b(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/Walking_Barrage/Orbital_Walking_Barrage_Data.Orbital_Walking_Barrage_Data'"));
	if (b.Object) Stratagems.Add(b.Object);
	static ConstructorHelpers::FObjectFinder<UStratagemData> c(TEXT("/Script/HellDivers2.StratagemData'/Game/HellDivers2/Stratagem/Orbit/Laser/Orbital_Laser_Data.Orbital_Laser_Data'"));
	if (c.Object) Stratagems.Add(c.Object);
	//실제 게임 시뮬시 주석

	static ConstructorHelpers::FObjectFinder<ULevelSequence> LS_SpawnFromHellpodRef(TEXT("/Script/LevelSequence.LevelSequence'/Game/HellDivers2/Sequencer/LS_SpawnHellpodCameraAnim.LS_SpawnHellpodCameraAnim'"));
	if(LS_SpawnFromHellpodRef.Object) LS_SpawnFromHellpod = LS_SpawnFromHellpodRef.Object;

	Tags.Add(TAG_PLAYER);
}

void APlayerCharacter::Test()
{
	//UGameplayCamerasSubsystem* fds = NewObject<UGameplayCamerasSubsystem>();
	//FCameraAnimationParams AnimParam;
	//fds->PlayCameraAnimation(DiversController, SpawnFromHellpodCameraAnim, AnimParam);
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

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HellDivers2/Characters/Player/Input/IA_Interact.IA_Interact'"));
	if (IA_InteractRef.Object)	IA_Interact = IA_InteractRef.Object;
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_PlayerRebirthRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_RebirthPlayer.MT_RebirthPlayer'"));
	if (MT_PlayerRebirthRef.Object)	MT_PlayerRebirth = MT_PlayerRebirthRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_PlayerReadyRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_PlayerReady.MT_PlayerReady'"));
	if (MT_PlayerReadyRef.Object)	MT_PlayerReady = MT_PlayerReadyRef.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MT_ToInteractConsoleRef(TEXT("/Script/Engine.AnimMontage'/Game/HellDivers2/Characters/Player/EditedAnimations/MT_PlayerReady.MT_PlayerReady'"));
	if (MT_ToInteractConsoleRef.Object)	MT_ToInteractConsole = MT_ToInteractConsoleRef.Object;
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
	SceneCaptureComp->SetRelativeLocation(FVector(170.0f, 78.0f, 18.0f));
	SceneCaptureComp->SetRelativeRotation(FRotator(5.0f, 205.0f, 3.0f));
	SceneCaptureComp->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComp->OrthoWidth = 170.0f;

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> CharCaptureTextureRef(TEXT("/Game/HellDivers2/UI/Source/LoadOut/RT_CameraView.RT_CameraView"));
	if(CharCaptureTextureRef.Succeeded()) SceneCaptureComp->TextureTarget = CharCaptureTextureRef.Object;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_GrayscaleRef(TEXT("/Script/Engine.Material'/Game/HellDivers2/Material/M_Grayscale.M_Grayscale'"));
	if (M_GrayscaleRef.Succeeded()) M_Grayscale = M_GrayscaleRef.Object;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BeginWeaponEquip();
	 
	////실제 게임 시뮬
	FString LevelName = GetLevel()->GetOuter()->GetName();
	if (LevelName == "InGameTestmap")
	{
		//GetCharacterMovement()->SetMovementMode(MOVE_None);
		//BeginWeaponEquip();
	}
	//else
	//{
	//}
	////실제 게임 시뮬
	
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	DiversController = CastChecked<ADiversPlayerController>(NewController);
	if (!DiversController)
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(DiversController->GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultMappingContext, 0);

	FString LevelName = GetLevel()->GetOuter()->GetName();

	//실제 게임 시뮬
	if (LevelName == "InGameTestmap")
	{
		//bActiveLookAction = false;
		//CameraBoom->bDoCollisionTest = false;
		//FollowCamera->SetRelativeLocation(FVector(550.0, 0.0f, 500.0f));
		//FollowCamera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

		//GetCharacterMovement()->GravityScale = 0.0f;

		SetStratagemFromGInst();
	}
	else
	{
		Summoned();
		SceneCaptureComp->ShowOnlyActorComponents(this, true);
	}
	//실제 게임 시뮬
}

void APlayerCharacter::Summoned()	//Rebirth 애니메이션 재생 후 호출 될 함수
{
	bActiveLookAction = true;
	CameraBoom->bDoCollisionTest = true;

	FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FollowCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SetCameraData(CameraDataManager[0]);

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->GravityScale = 1.0f;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FindItem();

	CheckDiveLanding();

	SetImpactPoint();
	
	if(Stratagems.Num() > 0) CalStratagemCoolTime(DeltaTime);

	//if (CurrentPose == EPose::Prone && bRightButton)
	//{
	//	if(HandleItem)
	//		FollowCamera->SetWorldRotation(HandleItem->GetActorQuat());
	//}


}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AInteractObj* DroppedItem = Cast<AInteractObj>(OtherActor);
	if (DroppedItem)
	{
		OverlappedObj.Add(DroppedItem);
	}
}

void APlayerCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInteractObj* temp = Cast<AInteractObj>(OtherActor);
	OverlappedObj.Remove(temp);
}

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
	EnhancedInputComponent->BindAction(IA_Heal, ETriggerEvent::Started, this, &APlayerCharacter::UseSyringe);

	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftButtonStarted);
	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftButtonTriggered);
	EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Completed, this, &APlayerCharacter::LeftButtonEnd);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &APlayerCharacter::Zoom);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &APlayerCharacter::Zoom);
	EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeWeaponAction);
	EnhancedInputComponent->BindAction(TakeItemAction, ETriggerEvent::Started, this, &APlayerCharacter::InteractItem);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
	EnhancedInputComponent->BindAction(IA_TakeStratagemBall, ETriggerEvent::Started, this, &APlayerCharacter::TakeStratagemBall);
	EnhancedInputComponent->BindAction(IA_TakeStratagemBall, ETriggerEvent::Completed, this, &APlayerCharacter::TakeStratagemBall);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallW, ETriggerEvent::Started, this, &APlayerCharacter::InputMacro);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallS, ETriggerEvent::Started, this, &APlayerCharacter::InputMacro);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallD, ETriggerEvent::Started, this, &APlayerCharacter::InputMacro);
	EnhancedInputComponent->BindAction(IA_InputStratagemBallA, ETriggerEvent::Started, this, &APlayerCharacter::InputMacro);
	//EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

	EnhancedInputComponent->BindAction(IA_Escape, ETriggerEvent::Started, this, &APlayerCharacter::Esc);
}

void APlayerCharacter::ChangeWeaponAction(const FInputActionValue& Value)
{
	const int32 WeaponIndex = Value.GetMagnitude();

	if (WeaponIndex == int32(EItemType::Projectile))
	{
		TakeOutGrenade();
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

void APlayerCharacter::TakeOutGrenade()
{
	if (GrenadeNum <= 0)
		return;

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;

	if (HandleItem)
	{
		PlayAnimMontage(HandleItem->GetInsertMontage());

		FOnMontageEnded MT_EndDelegate;
		MT_EndDelegate.BindLambda([this, SpawnParam](UAnimMontage* Montage, bool bInterrupted)
			{
				AttachToSocket(HandleItem, HandleItem->GetSocketName());
				PreItem = HandleItem;

				HandleItem = GetWorld()->SpawnActor<AItem>(GrenadeC, SpawnParam);

				AttachToSocket(HandleItem, HandSocketName);
				PlayAnimMontage(HandleItem->GetTakeOutMontage());
			}
		);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MT_EndDelegate);
	}
	else
	{
		HandleItem = GetWorld()->SpawnActor<AItem>(GrenadeC, SpawnParam);

		AttachToSocket(HandleItem, HandSocketName);
		PlayAnimMontage(HandleItem->GetTakeOutMontage());
	}
}

void APlayerCharacter::ThrowItem()
{
	FRotator Direction = GetControlRotation();
	Direction.Pitch += 10.0f;

	HandleItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HandleItem->Throw(Direction.Vector() * 2000.0f);
	HandleItem = nullptr;

	AStratagemBall* StratagemBall = Cast<AStratagemBall>(HandleItem);
	if (StratagemBall)
	{
		Direction.Pitch = 0.0f;
		StratagemBall->SetAbsoluteForwardVector(Direction);
	}

	SetLookingForward(true);

	GetMesh()->GetAnimInstance()->Montage_Play(MT_ThrowStratagemBall, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
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
		OnBulletEnemyHit.BindLambda([HUDWidget]() { HUDWidget->PlayEnemyHitAnim(); });

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

		OnShowLoadOutWidget.AddUObject(LoadOutWidget, &ULoadOutWidget::VisibleWidget);
		OnShowLoadOutWidget.AddUObject(DiversController, &ADiversPlayerController::SetMouseCursor);

		OnCloseStratagemSettingWidget.BindUObject(LoadOutWidget, &ULoadOutWidget::ExitSettingStratagem);
	}
}

void APlayerCharacter::SetStratagemFromGInst()
{
	UHelldivers2Instance* GInst = Cast<UHelldivers2Instance>(GetGameInstance());
	if (GInst)
	{
		TArray<UStratagemData*> StratagemDatas = GInst->GetTempStratagemsD();
		for (auto StratagemData : StratagemDatas)
		{
			Stratagems.Add(StratagemData);
		}

		UStratagemData* ResupplyStratagemData = LoadObject<UStratagemData>(nullptr, TEXT("/Game/HellDivers2/Stratagem/Resupply/Resupply.Resupply"));
		if (ResupplyStratagemData) Stratagems.Add(ResupplyStratagemData);
		ResupplyStratagemData = LoadObject<UStratagemData>(nullptr, TEXT("/Game/HellDivers2/Stratagem/SupplyWeapon/GR-8_RECOILESS.GR-8_RECOILESS"));
		if (ResupplyStratagemData) Stratagems.Add(ResupplyStratagemData);
		ResupplyStratagemData = LoadObject<UStratagemData>(nullptr, TEXT("/Game/HellDivers2/Stratagem/Orbit/Precision_Strike_Data/Orbital_Precision_Strike_Data.Orbital_Precision_Strike_Data"));
		if (ResupplyStratagemData) Stratagems.Add(ResupplyStratagemData);

		//if(OnStratagemSet.IsBound()) OnStratagemSet.Execute(Stratagems);
	}
}

void APlayerCharacter::CalStratagemCoolTime(float DeltaTime)
{
	for (auto Stratagem : Stratagems)
	{
		if (!Stratagem.IsNull() && Stratagem->bCoolTime)
		{
			Stratagem->CoolTime -= DeltaTime;

			FString CoolTimeStr = FString::Printf(TEXT("%d"), (int)Stratagem->CoolTime);
			Stratagem->OnSetCoolTimeText.Execute(CoolTimeStr);

			if (Stratagem->CoolTime <= 0.0f)
			{
				Stratagem->bCoolTime = false;
				Stratagem->OnHideMacroBox.Execute(false);
				Stratagem->OnActiveWidget.Execute(true);
			}
		}
	}
}

void APlayerCharacter::StratagemInput(uint8 InputMacro)
{
	bool bAllInactive = true;

	for (auto Stratagem : Stratagems)
	{
		if (!Stratagem->IsActive() || MacroIndex >= Stratagem->Macro.Num())
		{
			if (Stratagem->IsActive())
				Stratagem->SetbActive(false);
			continue;
		}
		else if (Stratagem->bCoolTime)
		{

		}
		else
		{
			const uint8 MacroKey = Stratagem->Macro[MacroIndex];
			if (MacroKey == InputMacro)
			{
				bAllInactive = false;

				const uint8 MacroN = Stratagem->Macro.Num();
				if (MacroIndex == MacroN - 1)	//입력한 키가 마지막 매크로일 경우
				{
					Stratagem->CorrectMacro(MacroIndex);

					bSucceededStratagem = true;

					UGameplayStatics::PlaySoundAtLocation(this, SW_BallLoopEnter, GetActorLocation());

					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					SetLookingForward(false);

					OnSetStratagemCoolTime.BindUObject(Stratagem, &UStratagemData::SetCoolTime);
					Stratagem->OnHideMacroBox.Execute(true);

					Cast<AStratagemBall>(HandleItem)->SetStratagem(Stratagem->GetCStratagem(), (uint8)Stratagem->GetStratagemType());

					for (int i = 0; i < Stratagems.Num(); i++)
					{
						if (Stratagems[i] != Stratagem)
							Stratagems[i]->ShowConditionWidgetDelegate.Execute(false);
					}

					return;
				}
				else //옳은 키를 입력했을 때
				{
					Stratagem->CorrectMacro(MacroIndex);
				}
			}
			else Stratagem->SetbActive(false);
		}
	}

	if (!bAllInactive)
		MacroIndex++;
	else
	{
		MacroIndex = 0;

		for (auto Stratagem : Stratagems)
			if (!Stratagem->bCoolTime) Stratagem->SetbActive(true);
	}
}

void APlayerCharacter::Die()
{
	//UE_LOG(LogTemp, Log, TEXT("You Die"));

	FollowCamera->PostProcessSettings.AddBlendable(M_Grayscale, 1.0f);
	FollowCamera->PostProcessBlendWeight = 1.0f;

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle RespawnTimeHandle;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimeHandle, this, &APlayerCharacter::Respawn, 3.0f, false);
}

void APlayerCharacter::SpawnFromHellpod()
{
	ALevelSequenceActor* OutActor;

	FMovieSceneSequencePlaybackSettings PlaybackSet;
	PlaybackSet.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;

	ULevelSequencePlayer* LSPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LS_SpawnFromHellpod,
		PlaybackSet,
		OutActor
	);
	OutActor->SequencePlayer->Play();

	PlayAnimMontage(MT_PlayerRebirth);

	FOnMontageEnded OnReadyMontageEnd;
	OnReadyMontageEnd.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		bActiveLookAction = true;
		CameraBoom->bDoCollisionTest = true;
		
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->GravityScale = 1.0f;
		});
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnReadyMontageEnd);
}

void APlayerCharacter::Respawn()
{
	//UE_LOG(LogTemp, Log, TEXT("Respawn"));
	Stat->SetHp(Stat->GetMaxHp());

	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -93.0f), FRotator(0.0f, 90.0f, 0.0f));

	FollowCamera->PostProcessSettings.RemoveBlendable(M_Grayscale);

	OnRespawnPlayer.Execute(this);
}

void APlayerCharacter::SetPlayerStratagem(UStratagemData* SData)
{
	Stratagems.Add(SData);
}

void APlayerCharacter::EnterHellpodBridge(AActor* BridgeHellpod)
{
	bActiveLookAction = false;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	FVector StartLoc = BridgeHellpod->GetActorLocation();
	FVector ForwardVec = -(BridgeHellpod->GetActorRightVector());
	StartLoc += ForwardVec * 200.0f;
	StartLoc.Z += 95.0f;

	FRotator GoalRot = BridgeHellpod->GetActorRotation();
	GoalRot.Yaw += 90.0f;

	SetActorRotation(GoalRot);
	SetActorLocation(StartLoc);

	PlayAnimMontage(MT_PlayerReady);

	ALevelSequenceActor* OutActor;

	FMovieSceneSequencePlaybackSettings PlaybackSet;
	PlaybackSet.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	PlaybackSet.bPauseAtEnd = true;

	ULevelSequencePlayer* LSPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LS_ToStratagemSetting,
		PlaybackSet,
		OutActor
	);
	OutActor->SequencePlayer->Play();
	OutActor->SequencePlayer->Pause();

	FOnMontageEnded OnMontageEnd;
	OnMontageEnd.BindLambda([this, BridgeHellpod, OutActor](UAnimMontage* Montage, bool bInterrupted) {
		OnShowLoadOutWidget.Broadcast(true);
		OutActor->SequencePlayer->Play();

		AttachToActor(BridgeHellpod, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hellpod_pad_Socket"));

		GetCharacterMovement()->GravityScale = 0.0f;
		});
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnd);

}

void APlayerCharacter::SetNearbyInteractable(AActor* Object)
{
	NearbyObj = Object;
}

void APlayerCharacter::SetStratagemConditionWidget(UUserWidget* InStratagemNoticeWidget)
{
	UW_StratagemCondition* W_StratagemCondition = Cast<UW_StratagemCondition>(InStratagemNoticeWidget);
	if (W_StratagemCondition)
	{
		//FOnShowConditionWidget ShowConditionWidgetDelegate;
		//ShowConditionWidgetDelegate.BindUObject(W_StratagemCondition, &UW_StratagemCondition::ShowWidget);
		//OnShowConditionDelegates.Add(ShowConditionWidgetDelegate);

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
		W_StratagemNotice->SetStratagemWidget(Stratagems);
		/*OnStratagemSet.BindUObject(W_StratagemNotice, &UW_StratagemNotice::SetStratagemWidget);
		if (OnStratagemSet.IsBound()) OnStratagemSet.Execute(Stratagems);*/
	}
}

void APlayerCharacter::LoadLevel(FName InLevelName)
{
	//UE_LOG(LogTemp, Log, TEXT("Load %s"), *InLevelName.ToString());
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
	if (bActiveLookAction)
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		rotateValue.X += LookAxisVector.X;
		rotateValue.Y += LookAxisVector.Y;
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
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
		//HandleItem->GetSkelMeshComp()->SetNotifyRigidBodyCollision(true);
		OnSetStratagemCoolTime.Execute(GetWorld());
		OnSetStratagemCoolTime.Unbind();		

		ThrowItem();

		if (PreItem)
		{
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
		}
		else HandleItem = nullptr;

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

					TakeOutGrenade();
					/*HandleItem = GetWorld()->SpawnActor<AItem>(GrenadeC);

					AttachToSocket(HandleItem, HandSocketName);
					PlayAnimMontage(HandleItem->GetTakeOutMontage());*/
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
		IGunInterface* GunInterface = Cast<IGunInterface>(HandleItem);
		if (GunInterface)
		{
			GunInterface->StopShotSound();
		}
		else if (!bRightButton)
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
			//ImpactWidget->SetHiddenInGame(false);

			IGunInterface* HandleGun = Cast<IGunInterface>(HandleItem);
			//ImpactWidget->SetWorldLocation(HandleGun->GetImpactPoint());
		}
		else
		{
			//ImpactWidget->SetHiddenInGame(true);
		}
	}
	else
	{
		//ImpactWidget->SetHiddenInGame(true);
	}
}

void APlayerCharacter::Interact()
{
	//IObjectInterface* Object = Cast<IObjectInterface>(NearbyObj);
	//if (Object)
	//{
	//	Object->Interact(this);
	//}
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
			Stratagems[i]->ShowConditionWidgetDelegate.Execute(true);
			Stratagems[i]->SetbActive(true);
		}

		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = this;
		AItem* StratagemBall = GetWorld()->SpawnActor<AItem>(StratagemBallC, SpawnParam);

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
				for (auto Stratagem : Stratagems) Stratagem->ShowConditionWidgetDelegate.Execute(false);
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

void APlayerCharacter::InputMacro(const FInputActionValue& Value)
{
	if (bSucceededStratagem || !bActivatedStratagemBall || !bActivatedConsole)
		return;

	PlayAnimMontage(MT_StratagemInput);
	UGameplayStatics::PlaySoundAtLocation(this, SW_BallArrow, GetActorLocation());

	const uint8 InputMacro = Value.Get<float>();

	if (bActivatedStratagemBall)
		StratagemInput(InputMacro);
	else if(bActivatedConsole)


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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SetCameraData"));
	CameraBoom->TargetArmLength = CameraData->TargetArmLength;
	CameraBoom->SocketOffset = CameraData->SocketOffset;


	FollowCamera->SetRelativeRotation(CameraData->Rot);
}

void APlayerCharacter::InteractItem(const FInputActionValue& Value)
{
	if (!OverlappedObj.IsEmpty())
	{
		AInteractObj* Obj = OverlappedObj.Pop();
		if (AItem* Item = Cast<AItem>(Obj))
		{
			FTakeItemDelegateWrapper* OnItemAction = TakeItemActions.Find(Item->GetItemType());
			if (Item && OnItemAction)
			{
				PlayAnimMontage(MT_GetItem);

				if (OnItemAction->ItemDelegate.IsBound()) OnItemAction->ItemDelegate.ExecuteIfBound(Item);
			}
		}
		else if (IObjectInterface* Object = Cast<IObjectInterface>(Obj))
		{
			Object->Interact(this);
		}

		Obj->SetActiveOverlapEvent(false);
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

void APlayerCharacter::UseSyringe(const FInputActionValue& Value)
{
	if (SyringeNum)
	{
		if (HandleItem)
			HandleItem->SetActorHiddenInGame(true);

		AItem* SpawnedSyringe = GetWorld()->SpawnActor<AItem>(SyringeC);
		AttachToSocket(SpawnedSyringe, HandSocketName);

		PlayAnimMontage(MT_UseHeal);
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindLambda([this, SpawnedSyringe](UAnimMontage* Montage, bool bInterrupted)
			{
				SpawnedSyringe->Destroy();
				HandleItem->SetActorHiddenInGame(false);
			});
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded);
	}
}


void APlayerCharacter::Heal()
{
	Stat->SetHp(Stat->GetMaxHp());

	SyringeNum--;

	if (Stat)
		Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
}

void APlayerCharacter::SetInteractConsole()
{
	bActivatedConsole = true;

	PlayAnimMontage(MT_ToInteractConsole);
}

void APlayerCharacter::EquipWeapon(AItem* NewItem)
{
	if (NewItem)
	{
		EItemType NewItemType = NewItem->GetItemType();

		if (Weapons.Find(NewItemType))
		{
			AItem* InventoryItem = *Weapons.Find(NewItemType);
			Weapons.Remove(NewItemType);
			InventoryItem->SetOwner(nullptr);
			InventoryItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			if (HandleItem == InventoryItem)
				HandleItem = nullptr;
		}

		if (!HandleItem)
		{
			HandleItem = NewItem;
			AttachToSocket(NewItem, HandSocketName);
		}
		else
		{
			AttachToSocket(NewItem, NewItem->GetSocketName());
		}

		Weapons.Add(NewItemType, NewItem);
		NewItem->OnPickedUp.Execute(true);
		NewItem->SetOwner(this);
	}
}

void APlayerCharacter::ChargeItem(AItem* Item)
{
	//UE_LOG(LogTemp, Warning, TEXT("소비 아이템은 보급으로만 충전 가능"));

	ChargeConsumedItem();
	Item->Destroy();
}

void APlayerCharacter::ChargeConsumedItem()
{
	SyringeNum = 4;
	GrenadeNum = 4;
	if (Weapons.Contains(EItemType::Main))
	{
		GunsMagazineN[EItemType::Main] = 4;
	}
	if (Weapons.Contains(EItemType::Pistol))
	{
		GunsMagazineN[EItemType::Pistol] = 4;
	}
	if (Weapons.Contains(EItemType::Stratagem))
	{
		GunsMagazineN[EItemType::Stratagem] = 4;
	}

	Stat->OnItemChanged.Broadcast(GrenadeNum, GunsMagazineN[HandleItem->GetItemType()], SyringeNum);
}

void APlayerCharacter::BeginWeaponEquip()
{
	GunsMagazineN.Add(EItemType::Main, 4);
	GunsMagazineN.Add(EItemType::Pistol, 4);
	GunsMagazineN.Add(EItemType::Projectile, 4);
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

void APlayerCharacter::FindItem()
{
	FVector StartLoc = FollowCamera->GetComponentLocation();
	FVector EndLoc = StartLoc + FollowCamera->GetForwardVector() * 550.0f;

	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(25.0f)
	);
	DrawDebugCylinder(GetWorld(), StartLoc, EndLoc, 25.0f, 4, FColor::Blue, false, 0.1f);

	if (bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *HitResult.GetActor()->GetName());
	}
}

void APlayerCharacter::AttachToSocket(AItem* Item, FName SocketName)
{
	FAttachmentTransformRules AttachmentTransformRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	//Item->GetSkelMeshComp()->SetSimulatePhysics(false);
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