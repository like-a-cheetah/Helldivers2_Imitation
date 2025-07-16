// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MacroWidgetComponent.generated.h"

/**
 * UMacroWidgetComponent
 *
 * - 매크로 암호 UI를 감싸고 입력 로직을 담당하는 컴포넌트입니다.
 * - 복잡한 상호작용 처리 및 매크로 성공 시 이벤트(Delegate)는 C++로 구현하여 신뢰성과 확장성을 확보했습니다.
 * - 간단한 연결, 연출, 후처리는 블루프린트에서 손쉽게 바인딩할 수 있도록 설계하여, 생산성과 유지보수를 높였습니다.
 *
 * 이 구조를 통해 퍼즐/콘솔 미니게임 등에서 C++의 강력한 로직 처리와
 * 블루프린트의 빠른 UI 연동 및 이벤트 처리를 모두 활용할 수 있습니다.
 */

 // 매크로 입력 성공 시 발생시킬 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccessMacro);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLDIVERS2_API UMacroWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UMacroWidgetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

public:
	// 플레이어 인터페이스를 통한 상호작용 진입 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void Interact(TScriptInterface<class IPlayerControl> Player);
	// C++ 코드 내에서 포인터 기반 인터페이스로 상호작용 진입
	void Interact(class IPlayerControl* Player);

	// 상호작용 종료 로직 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void Escape();

	UPROPERTY(BlueprintAssignable)
	FOnSuccessMacro OnSuccessMacro;

private:
	// 입력된 매크로와 정답 시퀀스 비교, 결과 반환
	uint8 CompareMacro(uint8 InMacro);

	// 상호작용 시작 기본 세팅 함수
	void BeginInteract(IPlayerControl* Player);

	// 정답 매크로(입력 시퀀스), 에디터 및 블루프린트에서 설정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true));
	TArray<uint8> Macro;

	// 콘솔 UI 위젯 객체 참조
	TObjectPtr<class UConsoleWidget> ConsoleWidget;
	
	// 현재 매크로 입력 인덱스
	uint8 CurMacroInd;
};
