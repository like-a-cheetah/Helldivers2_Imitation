#include "Characters/Components/CharacterStatComponent.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	MaxHp = 100.f;
	Hp = MaxHp;
}

void UCharacterStatComponent::SetMaxHp(float NewMaxHP)
{
	MaxHp = NewMaxHP;
	Hp = MaxHp;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	Hp = FMath::Clamp<float>(NewHp, 0, MaxHp);
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	UE_LOG(LogTemp, Log, TEXT("%s attacked %f Damage"), *GetOwner()->GetName(), InDamage);
	if (Hp <= 0.0f) return -1.0f;

	float PrevHp = Hp;
	float ActualDamage = FMath::Clamp(InDamage, 0.0f, PrevHp);
	//float ActualDamage = 1.f;

	SetHp(PrevHp - ActualDamage);	

	if (Hp <= 0.f)
	{
		if (OnHpZero.IsBound())
		{
			//UE_LOG(LogTemp, Log, TEXT("Bound"));
			OnHpZero.Broadcast();
		}
	}

	return ActualDamage;
}

