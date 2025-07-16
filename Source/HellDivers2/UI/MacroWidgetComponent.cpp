// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MacroWidgetComponent.h"

#include "ConsoleWidget.h"

#include "Interface/PlayerControl.h"

UMacroWidgetComponent::UMacroWidgetComponent(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UConsoleWidget> ConsoleWidgetRef(TEXT("/Game/HellDivers2/UI/Console/WBP_Console.WBP_Console_C"));
	if (ConsoleWidgetRef.Succeeded()) SetWidgetClass(ConsoleWidgetRef.Class);

	SetBlendMode(EWidgetBlendMode::Transparent);
}

void UMacroWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	ConsoleWidget = Cast<UConsoleWidget>(GetUserWidgetObject());
	if (ConsoleWidget)
	{	
		// UI에 매크로 암호에 일치하는 화살표 위젯 설정
		ConsoleWidget->SetMacro(Macro);
	}
}

void UMacroWidgetComponent::BeginInteract(IPlayerControl* Player)
{
	if (!Player) return;

	// 플레이어의 상호작용 상태 설정
	Player->SetInteractConsole(true, this);

	// 플레이어의 입력 이벤트 바인딩
	Player->OnInputMacro.BindUObject(this, &UMacroWidgetComponent::CompareMacro);

	// 입력 인덱스 초기화
	CurMacroInd = 0;
}

// 블루프린트용 인터페이스 (TScriptInterface)
void UMacroWidgetComponent::Interact(TScriptInterface<class IPlayerControl> Player)
{
	BeginInteract(Player.GetInterface());
}

// C++ 포인터 버전
void UMacroWidgetComponent::Interact(IPlayerControl* Player)
{
	BeginInteract(Player);
}

void UMacroWidgetComponent::Escape()
{
	// 상호작용 시, 호출 시 모든 화살표 UI 색상 초기화
	ConsoleWidget->InitAllArrowsColor();
}

uint8 UMacroWidgetComponent::CompareMacro(uint8 InMacro)
{
	// 입력값이 정답이면,
	if (Macro[CurMacroInd] == InMacro)
	{
		// 화살표 색상 변경 및 인덱스 증가
		ConsoleWidget->SetArrowsCorrect(CurMacroInd++);
		// 전체 성공 시 성공 이벤트 브로드캐스트
		if (Macro.Num() == CurMacroInd)
		{
			ConsoleWidget->SetVisibility(ESlateVisibility::Hidden);
			OnSuccessMacro.Broadcast();

			return 2; // 상호작용 종료 반환 값
		}
		return 1; // 상호작용 지속 반환 값
	}
	else
	{
		CurMacroInd = 0;
		ConsoleWidget->InitAllArrowsColor(); // 실패 시 UI 색 초기화

		return 0;
	}
}
