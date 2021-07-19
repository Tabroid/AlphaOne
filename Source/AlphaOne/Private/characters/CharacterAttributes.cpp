// Copyright Epic Games, Inc. All Rights Reserved.

#include "characters/CharacterAttributes.h"
#include "characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributes::UCharacterAttributes()
	: Health(1.f)
	, MaxHealth(1.f)
    , HealthRegen(0.f)
    , Absorption(0.f)
	, Mana(0.f)
	, MaxMana(0.f)
    , ManaRegen(0.f)
	, AttackPower(1.0f)
    , CriticalChance(0.05f)
    , CriticalDamage(0.5f)
    , ArmorPenetration(0.f)
    , DamageAmplification(0.f)
    , HitRate(0.9f)
	, DefensePower(1.0f)
    , DamageReduction(0.f)
    , DodgeRate(0.05f)
	, MoveSpeed(1.0f)
    , BaseMoveSpeed(1.0f)
	, AttackSpeed(1.0f)
{
}

void UCharacterAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterAttributes, Health);
	DOREPLIFETIME(UCharacterAttributes, MaxHealth);
    DOREPLIFETIME(UCharacterAttributes, HealthRegen);
    DOREPLIFETIME(UCharacterAttributes, Absorption);
	DOREPLIFETIME(UCharacterAttributes, Mana);
	DOREPLIFETIME(UCharacterAttributes, MaxMana);
    DOREPLIFETIME(UCharacterAttributes, ManaRegen);
	DOREPLIFETIME(UCharacterAttributes, AttackPower);
    DOREPLIFETIME(UCharacterAttributes, CriticalChance);
    DOREPLIFETIME(UCharacterAttributes, CriticalDamage);
    DOREPLIFETIME(UCharacterAttributes, ArmorPenetration);
    DOREPLIFETIME(UCharacterAttributes, DamageAmplification);
    DOREPLIFETIME(UCharacterAttributes, HitRate);
	DOREPLIFETIME(UCharacterAttributes, DefensePower);
    DOREPLIFETIME(UCharacterAttributes, DamageReduction);
    DOREPLIFETIME(UCharacterAttributes, DodgeRate);
    DOREPLIFETIME(UCharacterAttributes, MoveSpeed);
	DOREPLIFETIME(UCharacterAttributes, BaseMoveSpeed);
    DOREPLIFETIME(UCharacterAttributes, AttackSpeed);
}

void UCharacterAttributes::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
                                                       float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    // TODO
    return;
}

void UCharacterAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}