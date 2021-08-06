// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnitAttributes.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributes::UCharacterAttributes()
{
    // unset all flags for action and status
    Action = static_cast<EUnitActions>(0);
    Status = static_cast<EUnitStatuses>(0);
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

	if (Attribute == GetMaxHealthAttribute()) {
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	} else if (Attribute == GetMaxManaAttribute()) {
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

void UCharacterAttributes::RegenOverTime(float DeltaTime)
{
    RegenTimer += DeltaTime;
    if (RegenTimer < ALPHAONE_UNIT_REGEN_TICK) {
        return;
    }

    InitHealth(std::min(GetMaxHealth(), GetHealth() + GetHealthRegen()*RegenTimer));
    InitMana(std::min(GetMaxMana(), GetMana() + GetManaRegen()*RegenTimer));
    InitAbsorption(std::min(GetMaxAbsorption(), GetAbsorption() + GetAbsorptionRegen()*RegenTimer));

    RegenTimer = 0.;
}

void UCharacterAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCharacterAttributes, RotationRate);
    DOREPLIFETIME(UCharacterAttributes, JogSpeed);
    DOREPLIFETIME(UCharacterAttributes, SprintSpeed);

    DOREPLIFETIME(UCharacterAttributes, Health);
    DOREPLIFETIME(UCharacterAttributes, MaxHealth);
    DOREPLIFETIME(UCharacterAttributes, HealthRegen);
    DOREPLIFETIME(UCharacterAttributes, Absorption);
    DOREPLIFETIME(UCharacterAttributes, MaxAbsorption);
    DOREPLIFETIME(UCharacterAttributes, AbsorptionRegen);

    DOREPLIFETIME(UCharacterAttributes, Mana);
    DOREPLIFETIME(UCharacterAttributes, MaxMana);
    DOREPLIFETIME(UCharacterAttributes, ManaRegen);

    DOREPLIFETIME(UCharacterAttributes, AttackPower);
    DOREPLIFETIME(UCharacterAttributes, CriticalChance);
    DOREPLIFETIME(UCharacterAttributes, CriticalDamage);
    DOREPLIFETIME(UCharacterAttributes, ArmorPenetration);
    DOREPLIFETIME(UCharacterAttributes, DamageAmplification);
    DOREPLIFETIME(UCharacterAttributes, HitRate);
    DOREPLIFETIME(UCharacterAttributes, AttackSpeed);

    DOREPLIFETIME(UCharacterAttributes, Armor);
    DOREPLIFETIME(UCharacterAttributes, DamageReduction);
    DOREPLIFETIME(UCharacterAttributes, DodgeRate);
}

bool UCharacterAttributes::SetLevel(int32 NewValue)
{
    if (Level == NewValue) {
         return false;
    }
    Level = NewValue;
    return true;
}

void UCharacterAttributes::InitLevel(int32 NewValue)
{
    Level = NewValue;
}

bool UCharacterAttributes::LevelUp()
{
    Level += 1;
    return true;
    // @TODO, attributes increase
}

bool UCharacterAttributes::InitFromMetaDataTable(const UDataTable* DataTable, FName RowName)
{
    if (!DataTable) { return false; }

    auto Data = DataTable->FindRow<FUnitDataRow>(RowName, FString::Printf(TEXT("Query UnitData for %s ..."), *RowName.ToString()));

    if (!Data) { return false; }

    Name = RowName;
    InitLevel(Data->Level);
    InitRotationRate(Data->RotationRate);
    InitJogSpeed(Data->JogSpeed);
    InitSprintSpeed(Data->SprintSpeed);

    InitHealth(Data->Health);
    InitMaxHealth(Data->MaxHealth);
    InitHealthRegen(Data->HealthRegen);
    InitAbsorption(Data->Absorption);
    InitMaxAbsorption(Data->MaxAbsorption);
    InitAbsorptionRegen(Data->AbsorptionRegen);

    InitMana(Data->Mana);
    InitMaxMana(Data->MaxMana);
    InitManaRegen(Data->ManaRegen);

    InitAttackPower(Data->AttackPower);
    InitCriticalChance(Data->CriticalChance);
    InitCriticalDamage(Data->CriticalDamage);
    InitArmorPenetration(Data->ArmorPenetration);
    InitDamageAmplification(Data->DamageAmplification);
    InitHitRate(Data->HitRate);
    InitAttackSpeed(Data->AttackSpeed);

    InitArmor(Data->Armor);
    InitDamageReduction(Data->DamageReduction);
    InitDodgeRate(Data->DodgeRate);

    return true;
}
