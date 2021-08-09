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

// a helper macro to avoid trigger delegates while still in initialization
#define INIT_FROM_STRUCT(DataStruct, AttributeName) \
    AttributeName.SetCurrentValue(DataStruct->AttributeName); \
    AttributeName.SetBaseValue(DataStruct->AttributeName); \

    INIT_FROM_STRUCT(Data, RotationRate);
    INIT_FROM_STRUCT(Data, JogSpeed);
    INIT_FROM_STRUCT(Data, SprintSpeed);

    INIT_FROM_STRUCT(Data, Health);
    INIT_FROM_STRUCT(Data, MaxHealth);
    INIT_FROM_STRUCT(Data, HealthRegen);
    INIT_FROM_STRUCT(Data, Absorption);
    INIT_FROM_STRUCT(Data, MaxAbsorption);
    INIT_FROM_STRUCT(Data, AbsorptionRegen);

    INIT_FROM_STRUCT(Data, Mana);
    INIT_FROM_STRUCT(Data, MaxMana);
    INIT_FROM_STRUCT(Data, ManaRegen);

    INIT_FROM_STRUCT(Data, AttackPower);
    INIT_FROM_STRUCT(Data, CriticalChance);
    INIT_FROM_STRUCT(Data, CriticalDamage);
    INIT_FROM_STRUCT(Data, ArmorPenetration);
    INIT_FROM_STRUCT(Data, DamageAmplification);
    INIT_FROM_STRUCT(Data, HitRate);
    INIT_FROM_STRUCT(Data, AttackSpeed);

    INIT_FROM_STRUCT(Data, Armor);
    INIT_FROM_STRUCT(Data, DamageReduction);
    INIT_FROM_STRUCT(Data, DodgeRate);

    ExecuteHealthChangedDelegates(Health.GetCurrentValue(), Health.GetCurrentValue());
    ExecuteManaChangedDelegates(Mana.GetCurrentValue(), Mana.GetCurrentValue());
    ExecuteAbsorptionChangedDelegates(Absorption.GetCurrentValue(), Absorption.GetCurrentValue());

    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s Initialized, Bindings: %d!"), *RowName.ToString(), ManaChangedDelegates.Num()));
    return true;
}
