#include "Characters/Components/CharacterStatComponent.h"
#include "Characters/Player/PlayerCharacter.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	MaxHp = 100.f;
	Hp = MaxHp;
	bPlayerDead = false;
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterStatComponent::SetMaxHp(float NewMaxHP)
{
	MaxHp = NewMaxHP;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	Hp = FMath::Clamp<float>(NewHp, 0, MaxHp);
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	//float PrevHp = Hp;
	//float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	//SetHp(PrevHp - ActualDamage);

	//

	//if (Hp <= KINDA_SMALL_NUMBER)
	//{
	//	if (Cast<APlayerCharacter>(owner))
	//	{
	//		bPlayerDead = true;
	//	
	//		Cast<APlayerCharacter>(owner)->Revive();
	//	
	//	}
	//	else {
	//

	//		owner->RagdollSystemOperation();
	//		owner->DestroyWithTimer(4.f); // Ŀ���� �Լ�
	//		owner->SetDead(true);
	//	}


	//
	//
	//}
	//else
	//{
	//	bPlayerDead = false;
	//}

	//return ActualDamage;

	return 0.0f;
}

