// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h"
#include "InputAction.h"

#include "Interface/CharacterItemInterface.h"
#include "Interface/GunInterface.h"
#include "Items/EItemType.h"
#include "Interface/PlayerAnimInterface.h"
#include "EPose.h"
#include "Interface/PlayerControl.h"
#include "Interface/CharacterHUDInterface.h"
#include "Interface/StratagemInterface.h"

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

DECLARE_DELEGATE_OneParam(FOnActiveStratagemDelegate, bool /*bActive*/);
DECLARE_DELEGATE(FOnCloseStratagemSettingWidget);
DECLARE_DELEGATE_OneParam(FOnStratagemSet, TArray<class UStratagemData*> /*Stratagems*/);
DECLARE_DELEGATE_OneParam(FOnShowConditionWidget, bool /*bShow*/);
//ECLARE_DELEGATE_OneParam(FOnSetActiveW, bool /*bActive*/);

UCLASS()
class HELLDIVERS2_API APlayerCharacter : public ACharacter, public IPlayerAnimInterface, public IPlayerControl, public ICharacterHUDInterface, public IStratagemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	TObjectPtr<class ADiversPlayerController> DiversController;

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

protected:
	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void VaultObstacles(const FInputActionValue& Value);
	void Heal(const FInputActionValue& Value);
	void Diving();

	void LeftButtonStarted();
	void LeftButtonTriggered();
	void LeftButtonEnd();
	void Zoom(const FInputActionValue& Value);
	void ChangeWeaponAction(const FInputActionValue& Value);
	void TakeItem(const FInputActionValue& Value);
	void Reload();
	void TakeStratagemBall(const FInputActionValue& Value);
	void InputStratagemBall(const FInputActionValue& Value);

	void Esc();

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
	TArray<TObjectPtr<UAnimMontage>> MT_Divings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TArray<TObjectPtr<UAnimMontage>> MT_DivingLands;

// 사운드 관련
private:
	void SoundWaveFind();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StratagemBallClass, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundWave> SW_BallArrow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StratagemBallClass, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundWave> SW_BallLoopEnter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AudioComp)
	TObjectPtr<USoundWave> SW_DiveLand;

// 움직임 관련
protected:
	void SetSpeedFromCondition();

	UFUNCTION()
	bool TryGetObstacleHeight(float& WallHeight);

	UFUNCTION(BlueprintCallable)
	void SetLookingForward(bool bLookForward);

protected:
	uint8 bRightButton : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	uint8 bWalkableWall : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	uint8 bVault : 1;

	uint8 LShiftPressed : 1;

	FVector VaultStartPos;
	FVector VaultDestinationPos;
	float Height;

// 카메라 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TMap<int32, class UCameraData*> CameraDataManager;

	UPROPERTY(EditAnywhere, Category = Camera)
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComp;

private:
	void InitCameraSet();

	EPose CurrentPose;

	void ChangeCameraMode();
	void SetCameraData(const class UCameraData* CameraData);

// 아이템 관련
private:
	TObjectPtr<class UPlayerStatComponent> Stat;

	int32 SyringeNum;
	int32 GrenadeNum;
	TMap<EItemType, int32> GunsMagazineN;

	TArray<FString> GunClassPaths;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<class AItem*> GroundedItems;

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
	TSubclassOf<class AItem> Syringe;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AItem> Grenade;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

protected:
	void BeginWeaponEquip();

	void SwapWeapon(EItemType NextItemType);
	void EquipWeapon(class AItem* Item);
	void ChargeItem(class AItem* Item);

	void ChargeConsumedItem();

	void TakeOutGreade();

	void ThrowItem();

	UFUNCTION()
	void AttachToSocket(AItem* Item, FName SocketName);

	virtual void Recoil(float Yaw, float Pitch) override;

	UFUNCTION(BlueprintCallable, Category = Moving)
	void CheckDiveLanding();

	UFUNCTION(BlueprintCallable, Category = Moving)
	void SetImpactPoint();


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
	FORCEINLINE bool IsPullingPin() override { return bPullingPin; };
	FORCEINLINE bool IsRightButton() override { return bRightButton; }

	FVector2D rotateValue;

	// IPlayerControl을(를) 통해 상속됨
	void GetCurrentZ() override;

// 위젯 관련
protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = HUD)
	TObjectPtr<class UHUDWidget> HUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = HUD)
	TObjectPtr<class ULoadOutWidget> LoadOutWidget;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetComponent> ImpactWidget;

	// ICharacterHUDInterface을(를) 통해 상속됨
protected:
	virtual void SetupHUDWidget(UUserWidget* InHUDWidget) override;
	virtual void LoadLevel(FName InLevelName) override;
	virtual void SetStratagemsNoticeWidget(UUserWidget* InStratagemNoticeWidget) override;
	virtual void SetStratagemConditionWidget(UUserWidget* InStratagemNoticeWidget) override;

	//스트라타젬 관련
private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "Stratagem")
	TArray<TObjectPtr<class UStratagemData>> Stratagems;
	//TArray<TArray<uint8>> ActivatedMacros;

	uint8 MacroIndex;

	uint8 bActivatedStratagemBall : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StratagemBallClass, Meta = (AllowPrivateAccess = "true"))
	uint8 bSucceededStratagem : 1;

	UPROPERTY()
	TSubclassOf<class AItem> StratagemBallClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StratagemBallClass, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TestStratagemC;

	//FOnActiveStratagemDelegate OnActiveStratagem;
	FOnCloseStratagemSettingWidget OnCloseStratagemSettingWidget;
	FOnStratagemSet OnStratagemSet;
	TArray<FOnShowConditionWidget> OnShowConditionDelegates;
	//TArray<FOnSetActiveW> OnSetActiveWDelegates;

	void SetStratagemFromGInst();

	// IStratagemInterface을(를) 통해 상속됨
public:
	virtual void SetPlayerStratagem(class UStratagemData* SData) override;
};