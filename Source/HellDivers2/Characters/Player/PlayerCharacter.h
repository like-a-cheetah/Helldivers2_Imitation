// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "InputAction.h"

#include "Interface/CharacterItemInterface.h"
#include "Interface/GunInterface.h"
#include "Interface/PlayerAnimInterface.h"
#include "Interface/PlayerControl.h"
#include "Interface/CharacterHUDInterface.h"
#include "Interface/StratagemInterface.h"
#include "Items/EItemType.h"
#include "EPose.h"

#include "PlayerCharacter.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class AItem* /*Item*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

DECLARE_DELEGATE_OneParam(FOnCrosshairActive, bool /*bActive*/);

DECLARE_DELEGATE_OneParam(FOnRoundChange, float /*RoundRatio*/);
DECLARE_DELEGATE_OneParam(FOnHealActive, bool /*bHealing*/);

DECLARE_DELEGATE_OneParam(FOnActiveStratagemDelegate, bool /*bActive*/);
DECLARE_DELEGATE(FOnCloseStratagemSettingWidget);
//DECLARE_DELEGATE_OneParam(FOnStratagemSet, TArray<class UStratagemData*> /*Stratagems*/);
//DECLARE_DELEGATE_OneParam(FOnShowConditionWidget, bool /*bShow*/);
//ECLARE_DELEGATE_OneParam(FOnSetActiveW, bool /*bActive*/);
//DECLARE_DELEGATE_OneParam(FOnShowLoadOutWidget, bool /*bShow*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShowLoadOutWidget, bool /*bShow*/);
DECLARE_DELEGATE_OneParam(FOnSetStratagemCoolTime, UWorld*);
DECLARE_DELEGATE_OneParam(FOnExpansionMinimap, bool);

DECLARE_DELEGATE_OneParam(FOnRespawnPlayer, APlayerCharacter*);

DECLARE_DELEGATE_OneParam(FOnPlayStartGameSequencer, bool /*bPlayReverse*/);

DECLARE_DELEGATE(FOnEscapeCurrentSequence);

DECLARE_DELEGATE(FOnStratagemSettingClosed);

UCLASS()
class HELLDIVERS2_API APlayerCharacter : public ACharacter, public IPlayerAnimInterface, public IPlayerControl, public ICharacterHUDInterface, public IStratagemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	TObjectPtr<class ADiversPlayerController> DiversController;

public:
	FOnRespawnPlayer OnRespawnPlayer;

public:
	UFUNCTION(BlueprintCallable)
	void Test();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class ULevelSequence> LS_SpawnFromHellpod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class ULevelSequence> LS_ToStratagemSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class ULevelSequence> LS_ToSelectEarth;

	UFUNCTION(BlueprintCallable)
	void SelectLandPointSequence(ULevelSequencePlayer* Player, bool& bSetLandPoint);

	UFUNCTION(BlueprintCallable)
	void SelectStratagemSequence(ULevelSequencePlayer* SeqPlayer, bool& bSetLandPoint);

	TObjectPtr<class UUserWidget> LastHoveredWidget;

	FOnEscapeCurrentSequence OnEscapeCurrentSequence;
	FOnPlayStartGameSequencer OnPlayStartGameSequencer;

	FOnStratagemSettingClosed OnStratagemSettingClosed;

	uint8 bdWidgetInteractCompHover : 1;

// IA 관련
private:
	void InputActionFind();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DivingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Vault;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TakeItemAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Heal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WeaponChangeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_TakeStratagemBall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_InputStratagemBallW;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_InputStratagemBallS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_InputStratagemBallD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_InputStratagemBallA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Escape;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_InteractObj;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MinimapExpansion;

protected:
	void MoveStart();
	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void VaultObstacles(const FInputActionValue& Value);
	void UseSyringe(const FInputActionValue& Value);
	void Diving();

	void LeftButtonStarted();
	void LeftButtonTriggered();
	void LeftButtonEnd();
	void Zoom(const FInputActionValue& Value);
	void ChangeWeaponAction(const FInputActionValue& Value);
	void InteractItem(const FInputActionValue& Value);
	void TryReload();
	void TakeStratagemBall(const FInputActionValue& Value);
	void InputMacro(const FInputActionValue& Value);

	void Esc();
	void ExpansionMinimap(const FInputActionValue& Value);

private:
	uint8 bDive : 1;

public:
	void DivingLaunch();

// 몽타주 관련
private:
	void MontageFind();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Parkour_LowWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Parkour_HighWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Climb_LowWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Climb_HighWall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Climb_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_GetItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_UseHeal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_StandUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_StratagemInput;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_ThrowStratagemBall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_PullingPin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_PlayerRebirth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_PlayerReady;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_Start;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_CancelStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_PlayerCancelReady;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_InputConsole;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_SitPelican;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_FocusOnMissionTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<UAnimMontage> MT_FocusOutMissionTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UAnimMontage>> MT_Divings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UAnimMontage>> MT_DivingLands;

// 사운드 관련
private:
	void SoundWaveFind();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StratagemBallC, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundWave> SW_BallArrow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StratagemBallC, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundWave> SW_BallLoopEnter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AudioComp)
	TObjectPtr<USoundWave> SW_DiveLand;

// 움직임 관련
protected:
	void SetSpeedFromCondition();

	UFUNCTION()
	bool TryGetObstacleHeight(float& WallHeight, float& AnimAdjustZ);

	void FocusOnInteractObj(bool bFocus, FVector ObjLoc = FVector::ZeroVector);


public:
	void Stun(float StunTime);
	void EndStun();

	UFUNCTION(BlueprintCallable)
	void SetLookingForward(bool bLookForward);

protected:
	FVector2D InputVector;
	FVector2D LastInputVector;

	uint8 bRightButton : 1;
	uint8 bLeftButton : 1;
	
	uint8 bActiveRegIK : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	uint8 bWalkableWall : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	uint8 bVault : 1;

	uint8 LShiftPressed : 1;

	FVector VaultStartPos;
	FVector VaultDestinationPos;
	float Height;

	FRotator ImpactPointDirectRot;

// 카메라 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> SequenceCamera;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> MinimapSceneCaptureCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneCaptureComponent2D> MinimapSceneCaptureComp;

	TArray<float> MinimapCaptureOrthoWidths;


	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TMap<int32, class UCameraData*> CameraDataManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class USceneCaptureComponent2D> InHellpodSceneCaptrueComp;

	uint8 bActiveLookAction : 1;

private:
	void InitCameraSet();

	EPose CurrentPose;

	void ChangeCameraMode();
	void SetCameraData(const class UCameraData* CameraData);

	void UpdateSequenceCamera();

	UPROPERTY()
	TObjectPtr<UMaterialInterface> M_Grayscale;

public:
	UFUNCTION(BlueprintCallable)
	void SetActiveSequenceCamera(bool bActive);

// 아이템 관련
public:
	void SuccessReload();


private:
	UPROPERTY()
	TObjectPtr<class UPlayerStatComponent> Stat;

	int32 SyringeNum;
	int32 GrenadeNum;
	TMap<EItemType, int32> GunsMagazineN;

	TArray<FString> GunClassPaths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> NearbyObj;

	uint8 bActiveConsole : 1;
	
	UPROPERTY()
	TObjectPtr<class AInteractObj> InteractingObj;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSet<class AInteractObj*> OverlappedObj;

	FORCEINLINE int32 GetSyringeN() { return SyringeNum; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	uint8 bPullingPin : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class AItem> HandleItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TMap<EItemType, class AItem*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class AItem> PreItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AItem> SyringeC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AItem> GrenadeC;

	UPROPERTY()
	TMap<EItemType, FTakeItemDelegateWrapper> TakeItemActions;

	float HealDuration;

protected:
	void BeginWeaponEquip();

	void FindItem();

	void SwapWeapon(EItemType NextItemType);
	void EquipWeapon(class AItem* Item);
	void ChargeItem(class AItem* Item);

	void ChargeConsumedItem();

	void TakeOutGrenade();

	void ActiveSyringe(float DeltaTime);

	UFUNCTION()
	void AttachToSocket(AItem* Item, FName SocketName);

	virtual void Recoil(float Yaw, float Pitch) override;

	UFUNCTION(BlueprintCallable, Category = Moving)
	void CheckDiveLanding();

	UFUNCTION(BlueprintCallable, Category = Moving)
	void SetImpactPoint(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = Moving)
	void CalculateMuzzleDirectionRotation();

	void Interact();

	void InputConsole(float InputMacro);

	void InteractingGlobe(float DeltaTime);

	uint8 bInteractGlobe : 1;

	TObjectPtr<USceneComponent> GlobeComp;

	FVector2D PreMouseLoc;

	FTransform InitCameraRelativeTransform;

	UFUNCTION(BlueprintCallable)
	void DetachCamera(bool bDetach);

public:
	UFUNCTION(BlueprintCallable, Category = Weapon)
	FORCEINLINE AItem* GetHandleItem() { return HandleItem; }

	FName HandSocketName;

// 인터페이스 관련
public:
	// IPlayerAnimInterface을(를) 통해 상속됨
	EItemType GetCurrentItemType() override;
	FORCEINLINE EPose GetCurrentPose() override { return CurrentPose; }
	FORCEINLINE bool IsSucceededStratagem() override { return bSucceededStratagem; }
	FORCEINLINE bool IsPullingPin() override { return bPullingPin; }
	FORCEINLINE bool IsRightButton() override { return bRightButton; }
	FORCEINLINE bool IsLeftButton() override { return bLeftButton; }
	FORCEINLINE bool IsConsoleActive() override { return bActiveConsole; }
	FORCEINLINE bool IsRegIkActive() override { return bActiveRegIK; }
	FORCEINLINE FVector2D GetInputVector() override { return LastInputVector; }
	FORCEINLINE FRotator GetChestRot() override { return ImpactPointDirectRot; }

	// IPlayerControl을(를) 통해 상속됨
	void GetCurrentZ() override;
	void Summoned() override;
	void SetNearbyInteractable(AActor* Object) override;
	void EnterHellpodBridge(AActor* BridgeHellpod) override;
	void LeaveHellpodBridge(AActor* BridgeHellpod) override;
	void Heal() override;
	void SetInteractConsole(bool bInteractive, UMeshComponent* ConsoleWidget = nullptr) override;
	void SetFocusOnMissionTable(bool bInteract, class UGlobeComponent* GlobeComponent) override;

	float RidePelican(FTransform SitTr) override;
	void VaultPostProcess() override;
	FORCEINLINE void SetCurrentLVSequenceActor(class ALevelSequenceActor* InLVSequenceActor) override
	{ CurrentLVSequenceActor = InLVSequenceActor; } 

// 위젯 관련
public:
	FOnRoundChange OnRoundChange;
	FOnCrosshairActive OnCrosshairActive;
	FOnHealActive OnHealActive;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetInteractionComponent> WidgetInteractComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetComponent> ImpactPointWidget;

	// ICharacterHUDInterface을(를) 통해 상속됨
protected:
	virtual void SetupHUDWidget(UUserWidget* InHUDWidget) override;
	virtual void LoadBattleFieldLevel() override;
	virtual void SetStratagemsNoticeWidget(UUserWidget* InStratagemNoticeWidget) override;
	virtual void SetStratagemConditionWidget(UUserWidget* InStratagemNoticeWidget) override;

	//스트라타젬 관련
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Stratagem")
	TArray<TObjectPtr<class UStratagemData>> Stratagems;
	//TArray<TArray<uint8>> ActivatedMacros;

	uint8 MacroIndex;

	uint8 bActivatedStratagemBall : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StratagemBallC, Meta = (AllowPrivateAccess = "true"))
	uint8 bSucceededStratagem : 1;

	UPROPERTY()
	TSubclassOf<class AItem> StratagemBallC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StratagemBallC, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TestStratagemC;

	//FOnActiveStratagemDelegate OnActiveStratagem;
	FOnCloseStratagemSettingWidget OnCloseStratagemSettingWidget;
	//FOnStratagemSet OnStratagemSet;
	//TArray<FOnShowConditionWidget> OnShowConditionDelegates;
	//TArray<FOnSetActiveW> OnSetActiveWDelegates;
	FOnShowLoadOutWidget OnShowLoadOutWidget;
	FOnExpansionMinimap OnExpansionMinimap;

	void SetStratagemFromGInst();

	FOnSetStratagemCoolTime OnSetStratagemCoolTime;

	void CalStratagemCoolTime(float DeltaTime);

	void InputStratagem(uint8 InputMacro);

public:
	UFUNCTION()
	void Die();
	void SpawnFromHellpod();

	void PlayerInHellpodState();

	void Respawn();

	UFUNCTION(BlueprintCallable)
	void ThrowItem();

	// IStratagemInterface을(를) 통해 상속됨
public:
	virtual void SetPlayerStratagem(class UStratagemData* SData) override;

};